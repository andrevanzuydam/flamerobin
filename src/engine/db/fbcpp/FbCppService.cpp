/*
  Copyright (c) 2004-2026 The FlameRobin Development Team

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "engine/db/fbcpp/FbCppService.h"
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <firebird/Interface.h>
#include <firebird/impl/consts_pub.h>

extern "C" Firebird::IMaster* ISC_EXPORT fb_get_master_interface();

namespace
{

// Subclass to expose ServiceManager's protected action helpers so we can
// run isc_action_svc_properties synchronously without spawning a worker
// thread.
class PropertyServiceManager final : public fbcpp::ServiceManager
{
public:
    using fbcpp::ServiceManager::ServiceManager;
    using fbcpp::ServiceManager::startAction;
    using fbcpp::ServiceManager::waitForCompletion;
};

inline void appendInt32(std::vector<std::uint8_t>& spb, std::uint8_t tag, std::int32_t value)
{
    spb.push_back(tag);
    spb.push_back(static_cast<std::uint8_t>(value & 0xff));
    spb.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
    spb.push_back(static_cast<std::uint8_t>((value >> 16) & 0xff));
    spb.push_back(static_cast<std::uint8_t>((value >> 24) & 0xff));
}

inline void appendByte(std::vector<std::uint8_t>& spb, std::uint8_t tag, std::uint8_t value)
{
    spb.push_back(tag);
    spb.push_back(value);
}

std::vector<std::uint8_t> beginPropertiesSpb(const std::string& dbPath)
{
    std::vector<std::uint8_t> spb;
    spb.reserve(8 + dbPath.size());
    spb.push_back(static_cast<std::uint8_t>(isc_action_svc_properties));
    spb.push_back(static_cast<std::uint8_t>(isc_spb_dbname));
    const auto len = static_cast<std::uint16_t>(dbPath.size());
    spb.push_back(static_cast<std::uint8_t>(len & 0xff));
    spb.push_back(static_cast<std::uint8_t>((len >> 8) & 0xff));
    spb.insert(spb.end(), dbPath.begin(), dbPath.end());
    return spb;
}

} // namespace

namespace fr
{

namespace
{

// Run a one-shot properties action against the service manager and wait
// for it to finish. Synchronous because callers (e.g. the inline
// "Edit Sweep Interval" UI) expect the change to be applied before
// returning. Builds and tears down its own ServiceManager so the
// action is fully isolated from any other in-flight service work.
void runPropertiesAction(fbcpp::Client& client,
    const fbcpp::ServiceManagerOptions& options,
    const std::vector<std::uint8_t>& spb)
{
    PropertyServiceManager mgr(client, options);
    mgr.startAction(spb);
    mgr.waitForCompletion();
}

} // namespace

FbCppService::FbCppService()
{
}

void FbCppService::connect()
{
    if (!clientM)
    {
        Firebird::IMaster* master = fb_get_master_interface();
        if (!master)
            throw std::runtime_error("Failed to get Firebird master interface");
        clientM.emplace(master);
    }

    auto options = fbcpp::ServiceManagerOptions()
        .setServer(connStrM)
        .setUserName(userM)
        .setPassword(passwordM);

    serviceM.emplace(*clientM, options);
}

void FbCppService::disconnect()
{
    serviceM.reset();
}

void FbCppService::setConnectionString(const std::string& connStr)
{
    connStrM = connStr;
}

void FbCppService::setCredentials(const std::string& user, const std::string& password)
{
    userM = user;
    passwordM = password;
}

void FbCppService::setRole(const std::string& role)
{
    roleM = role;
}

void FbCppService::setCharset(const std::string& charset)
{
    charsetM = charset;
}

void FbCppService::setClientLibrary(const std::string& libraryPath)
{
    libraryPathM = libraryPath;
}

FbCppService::~FbCppService()
{
    if (serviceThreadM.joinable())
        serviceThreadM.join();
}

void FbCppService::pushLine(std::string_view line)
{
    std::lock_guard<std::mutex> lock(queueMutexM);
    outputQueueM.push(std::string(line));
}

void FbCppService::runService(std::function<void()> func)
{
    if (serviceThreadM.joinable())
        serviceThreadM.join();

    {
        std::lock_guard<std::mutex> lock(queueMutexM);
        while (!outputQueueM.empty())
            outputQueueM.pop();
    }

    serviceThreadM = std::thread(func);
}

void FbCppService::backup(const BackupConfig& config)
{
    if (!clientM)
        connect();

    auto options = fbcpp::BackupOptions()
        .setDatabase(config.dbPath)
        .addBackupFile(config.backupPath)
        .setVerboseOutput([this](std::string_view line) { pushLine(line); });
    
    if (config.parallel > 0)
        options.setParallelWorkers(static_cast<uint32_t>(config.parallel));

    runService([this, options]() {
        try
        {
            fbcpp::BackupManager manager(*clientM, fbcpp::ServiceManagerOptions()
                .setServer(connStrM)
                .setUserName(userM)
                .setPassword(passwordM));
            manager.backup(options);
        }
        catch (const std::exception& e)
        {
            pushLine(std::string("Error during backup: ") + e.what());
        }
        pushLine(""); // EOF marker
    });
}

void FbCppService::restore(const RestoreConfig& config)
{
    if (!clientM)
        connect();

    auto options = fbcpp::RestoreOptions()
        .setDatabase(config.dbPath)
        .addBackupFile(config.backupPath)
        .setReplace((int)config.flags & (int)RestoreFlags::Replace)
        .setVerboseOutput([this](std::string_view line) { pushLine(line); });

    if (config.parallel > 0)
        options.setParallelWorkers(static_cast<uint32_t>(config.parallel));

    runService([this, options]() {
        try
        {
            fbcpp::BackupManager manager(*clientM, fbcpp::ServiceManagerOptions()
                .setServer(connStrM)
                .setUserName(userM)
                .setPassword(passwordM));
            manager.restore(options);
        }
        catch (const std::exception& e)
        {
            pushLine(std::string("Error during restore: ") + e.what());
        }
        pushLine(""); // EOF marker
    });
}

void FbCppService::maintain(const MaintenanceConfig& config)
{
    throw std::runtime_error("Maintenance not implemented yet in FbCppService");
}

void FbCppService::shutdown(const ShutdownConfig& config)
{
    // Firebird 3.0+ shutdown using service manager is complex via low-level API.
    // fb-cpp doesn't have a direct wrapper yet, so we use a stub for now.
    // In a real implementation, we would use the low-level Service API.
    throw std::runtime_error("Shutdown not implemented yet in FbCppService");
}

void FbCppService::startup(const std::string& /*dbPath*/)
{
    throw std::runtime_error("Startup not implemented yet in FbCppService");
}

std::string FbCppService::getNextLine()
{
    std::lock_guard<std::mutex> lock(queueMutexM);
    if (outputQueueM.empty())
        return "";
    std::string line = outputQueueM.front();
    outputQueueM.pop();
    return line;
}

void FbCppService::getUsers(std::vector<UserData>& users)
{
    // Firebird user management via services uses a specific set of SPB items.
    // Since fb-cpp doesn't wrap this, we would need to go low-level.
    // For now, we'll keep it as a TODO or implement a basic version if possible.
    users.clear();
}

void FbCppService::addUser(const UserData& /*user*/)
{
}

void FbCppService::modifyUser(const UserData& /*user*/)
{
}

void FbCppService::removeUser(const std::string& /*username*/)
{
}

bool FbCppService::versionIsHigherOrEqualTo(int major, int minor)
{
    // Mocking for now, could be implemented using ServiceManager::getInfo
    return true; 
}

std::string FbCppService::getVersion()
{
    return "Firebird (fb-cpp)";
}

void FbCppService::setSweepInterval(const std::string& dbPath, int value)
{
    if (!clientM)
        connect();
    auto spb = beginPropertiesSpb(dbPath);
    appendInt32(spb, isc_spb_prp_sweep_interval, value);
    runPropertiesAction(*clientM,
        fbcpp::ServiceManagerOptions().setServer(connStrM)
            .setUserName(userM).setPassword(passwordM),
        spb);
}

void FbCppService::setPageBuffers(const std::string& dbPath, int value)
{
    if (!clientM)
        connect();
    auto spb = beginPropertiesSpb(dbPath);
    appendInt32(spb, isc_spb_prp_page_buffers, value);
    runPropertiesAction(*clientM,
        fbcpp::ServiceManagerOptions().setServer(connStrM)
            .setUserName(userM).setPassword(passwordM),
        spb);
}

void FbCppService::setSyncWrite(const std::string& dbPath, bool sync)
{
    if (!clientM)
        connect();
    auto spb = beginPropertiesSpb(dbPath);
    appendByte(spb, isc_spb_prp_write_mode,
        sync ? isc_spb_prp_wm_sync : isc_spb_prp_wm_async);
    runPropertiesAction(*clientM,
        fbcpp::ServiceManagerOptions().setServer(connStrM)
            .setUserName(userM).setPassword(passwordM),
        spb);
}

void FbCppService::setReserveSpace(const std::string& dbPath, bool reserve)
{
    if (!clientM)
        connect();
    auto spb = beginPropertiesSpb(dbPath);
    appendByte(spb, isc_spb_prp_reserve_space,
        reserve ? isc_spb_prp_res_use_full : isc_spb_prp_res);
    runPropertiesAction(*clientM,
        fbcpp::ServiceManagerOptions().setServer(connStrM)
            .setUserName(userM).setPassword(passwordM),
        spb);
}

void FbCppService::setReadOnly(const std::string& dbPath, bool readOnly)
{
    if (!clientM)
        connect();
    auto spb = beginPropertiesSpb(dbPath);
    appendByte(spb, isc_spb_prp_access_mode,
        readOnly ? isc_spb_prp_am_readonly : isc_spb_prp_am_readwrite);
    runPropertiesAction(*clientM,
        fbcpp::ServiceManagerOptions().setServer(connStrM)
            .setUserName(userM).setPassword(passwordM),
        spb);
}

} // namespace fr
