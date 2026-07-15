// Reproduces the flamerobin data-grid "delete row" flow against a real DB
// so we can catch DAL-level regressions without dragging in the GUI.
//
// Sequence intentionally mirrors DataGridRows::initialize+fetch+removeRows:
//   1. Prepare + execute SELECT with an active fetchable cursor.
//   2. Fetch a few rows (leaves cursor open — this is what the grid does).
//   3. Prepare a separate DELETE FROM ... WHERE ID = ? statement.
//   4. Bind + execute the DELETE.
//   5. Commit.
//
// If steps 3–5 crash or throw an unhandled exception, that's the same
// pattern the grid hits when the user presses Delete on a row.

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

#include "engine/db/DatabaseFactory.h"
#include "engine/db/IDatabase.h"
#include "engine/db/ITransaction.h"
#include "engine/db/IStatement.h"

int main()
{
    const char* envDb = std::getenv("FR_TEST_DB");
    const std::string dbName = envDb ? envDb
        : R"(localhost:D:\projects\c++\flamerobin\build64\RelWithDebInfo\frtest.fdb)";

    std::cout << "Connecting to: " << dbName << "\n";

    try
    {
        fr::IDatabasePtr db = fr::DatabaseFactory::createDatabase(
            fr::DatabaseBackend::FbCpp);
        db->setConnectionString(dbName);
        db->setCredentials("SYSDBA", "masterkey");
        db->setCharset("UTF8");
        db->connect();
        std::cout << "  Connected, engine: " << db->getEngineVersion() << "\n";

        fr::ITransactionPtr tr = db->createTransaction();
        tr->start();

        // --- Step 1-2: SELECT with active cursor, fetch several rows ---
        fr::IStatementPtr sel = db->createStatement(tr);
        sel->prepare(
            "SELECT ID, ACCOUNT_NO, STOCK_GROUP, PERCENTAGE, "
            "       GROUP_NAME, VENDOR_NAME, SUPPLIER_PERCENTAGE, "
            "       PRICEGROUPID, REP_PERCENTAGE "
            "  FROM PRICE_LOOKUP ORDER BY ID");
        sel->execute();

        int fetched = 0;
        std::vector<int> ids;
        while (fetched < 5 && sel->fetch())
        {
            int id = sel->getInt32(0);
            ids.push_back(id);
            std::cout << "  fetched row " << (fetched + 1) << " ID=" << id << "\n";
            ++fetched;
        }
        std::cout << "  fetched " << fetched << " rows, cursor still open\n";

        if (ids.empty())
        {
            std::cerr << "  no rows to delete — populate frtest.fdb first\n";
            return 1;
        }

        // --- Step 3-4: prepare DELETE and execute against first fetched row.
        // Same transaction, cursor left open (grid-like scenario).
        fr::IStatementPtr del = db->createStatement(tr);
        std::cout << "  preparing DELETE...\n";
        del->prepare("DELETE FROM PRICE_LOOKUP WHERE ID = ?");
        std::cout << "  binding param " << ids[0] << "...\n";
        del->setInt32(0, ids[0]);
        std::cout << "  executing DELETE...\n";
        del->execute();
        std::cout << "  DELETE affected " << del->getAffectedRows() << " row(s)\n";

        // --- Step 5: commit ---
        std::cout << "  committing...\n";
        tr->commit();

        std::cout << "  OK — no crash, no exception\n";

        // Also test the re-prepare pattern (previous FbCppStatement bug fixed
        // in commit 24c30cb2): SELECT + fetch, then re-prepare DELETE on the
        // SAME statement handle.
        std::cout << "Re-prepare pattern:\n";
        tr = db->createTransaction();
        tr->start();
        fr::IStatementPtr st = db->createStatement(tr);
        st->prepare("SELECT ID FROM PRICE_LOOKUP ORDER BY ID");
        st->execute();
        if (st->fetch())
        {
            int keepId = st->getInt32(0);
            std::cout << "  fetched first ID=" << keepId
                      << ", now re-preparing DELETE on same handle...\n";
            st->prepare("DELETE FROM PRICE_LOOKUP WHERE ID = ?");
            st->setInt32(0, keepId);
            st->execute();
            std::cout << "  re-prepared DELETE affected "
                      << st->getAffectedRows() << " row(s)\n";
        }
        tr->commit();
        std::cout << "  OK\n";

        db->disconnect();
        std::cout << "PASSED\n";
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "FAILED with std::exception: " << e.what() << "\n";
        return 2;
    }
    catch (...)
    {
        std::cerr << "FAILED with unknown exception\n";
        return 3;
    }
}
