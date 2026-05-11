/*
  Copyright (c) 2004-2022 The FlameRobin Development Team

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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/file.h>
#include <wx/tokenzr.h>

#include <algorithm>

#include "core/StringUtils.h"
#include "engine/db/IBlob.h"
#include "engine/db/ibpp/IbppService.h"
#include "frutils.h"
#include "gui/ProgressDialog.h"
#include "gui/UsernamePasswordDialog.h"
#include "metadata/column.h"
#include "metadata/relation.h"
#include "metadata/server.h"
#include "config/Config.h"

void adjustControlsMinWidth(std::list<wxWindow*> controls)
{
    int w = 0;
    wxSize sz;
    // find widest control
    for (std::list<wxWindow*>::iterator it = controls.begin();
        it != controls.end(); ++it)
    {
        wxASSERT(*it != 0);
        sz = (*it)->GetSize();
        w = std::max(w, sz.GetWidth());
    }
    // set minimum width of all controls
    for (std::list<wxWindow*>::iterator it = controls.begin();
        it != controls.end(); ++it)
    {
        sz = (*it)->GetSize();
        (*it)->SetSize(w, sz.GetHeight());
        (*it)->SetSizeHints(w, sz.GetHeight());
    }
}

void readBlob(fr::IStatementPtr& st, int column, wxString& result,
    wxMBConv* conv)
{
    result = "";
    if (st->isNull(column))
        return;

    // The fb-cpp backend strictly type-checks getString() against the
    // descriptor and throws "Invalid type: actual type std::string,
    // descriptor type 520" for SQL_BLOB columns. IBPP silently converted
    // them. Read the data via the BLOB API when the column actually is a
    // BLOB, and fall back to getString() for plain CHAR / VARCHAR
    // descriptions (some metadata views project descriptions as TEXT).
    if (st->getColumnType(column) == fr::ColumnType::Blob)
    {
        fr::IBlobPtr blob = st->getBlob(column);
        if (!blob)
            return;
        blob->open();
        const long len = blob->getLength();
        if (len > 0)
        {
            std::string buf;
            buf.resize(static_cast<size_t>(len));
            // Read in segments; getMaxSegmentSize() can be 0 for empty
            // streams or when the engine refuses to report — fall back
            // to one big read in that case.
            int segMax = blob->getMaxSegmentSize();
            if (segMax <= 0)
                segMax = static_cast<int>(len);
            size_t offset = 0;
            while (offset < buf.size())
            {
                int want = static_cast<int>(
                    std::min<size_t>(buf.size() - offset, segMax));
                int got = blob->read(&buf[offset], want);
                if (got <= 0)
                    break;
                offset += static_cast<size_t>(got);
            }
            buf.resize(offset);
            result = wxString(buf.c_str(), *conv, buf.size());
        }
        blob->close();
        return;
    }

    std::string s = st->getString(column);
    result = wxString(s.c_str(), *conv);
}

wxString selectRelationColumns(Relation* t, wxWindow* parent)
{
    std::vector<wxString> list;
    if (!selectRelationColumnsIntoVector(t, parent, list))
        return wxEmptyString;

    std::vector<wxString>::iterator it = list.begin();
    wxString retval(*it);
    while ((++it) != list.end())
        retval += ", " + (*it);
    return retval;
}

bool selectRelationColumnsIntoVector(Relation* t, wxWindow* parent,
    std::vector<wxString>& list)
{
    t->ensureChildrenLoaded();

    wxArrayInt selected_columns;
    wxArrayString colNames;
    colNames.Alloc(t->getColumnCount());
    for (ColumnPtrs::const_iterator it = t->begin(); it != t->end(); ++it)
        colNames.Add((*it)->getName_());

    // set default selection.
    for (std::vector<wxString>::const_iterator it = list.begin();
        it != list.end(); ++it)
    {
        wxString::size_type i = colNames.Index((*it));
        if (i != wxNOT_FOUND)
            selected_columns.Add(i);
    }

    bool ok =
    ::wxGetSelectedChoices(selected_columns,
        _("Select one or more fields... (use ctrl key)"),  _("Table Fields"),
        colNames, parent) > 0;
    list.clear();
    if (!ok)
        return false;

    for (size_t i = 0; i < selected_columns.GetCount(); ++i)
    {
        Identifier temp(colNames[selected_columns[i]]);
        list.push_back(temp.getQuoted());
    }
    return true;
}

bool connectDatabase(Database* db, wxWindow* parent,
    ProgressDialog* progressdialog)
{
    wxString pass(db->getDecryptedPassword());
    if (db->getAuthenticationMode().getAlwaysAskForPassword())
    {
        UsernamePasswordDialog upd(parent, wxEmptyString,
            db->getUsername(), UsernamePasswordDialog::Default);
        if (upd.ShowModal() != wxID_OK)
            return false;
        pass = upd.getPassword();
    }

    wxString caption(wxString::Format(_("Connecting to Database \"%s\""),
        db->getName_().c_str()));
    if (progressdialog)
    {
        progressdialog->setProgressMessage(caption);
        db->connect(pass, progressdialog);
    }
    else
    {
        ProgressDialog pd(parent, caption, 1);
        pd.setProgressMessage(caption);
        db->connect(pass, &pd);
    }
    return true;
}

bool getService(Server* s, IBPP::Service& svc, ProgressIndicator* p,
    bool sysdba)
{
    fr::IServicePtr dalSvc = s->getDALService(p, sysdba);
    if (!dalSvc)
    {
        wxString msg;
        if (p->isCanceled())
            msg = _("You have canceled the search for usable existing connection credentials.");
        else
            msg = _("None of the known database connection credentials could be used.");
        if (sysdba)
            msg = msg + "\n" + _("Please enter connection credentials with administrative rights.");

        int flags = UsernamePasswordDialog::AllowTrustedUser
            | (sysdba ? 0 : UsernamePasswordDialog::AllowOtherUsername);
        UsernamePasswordDialog upd(wxGetActiveWindow(), msg, "SYSDBA",
            flags);
        if (upd.ShowModal() != wxID_OK)
            return false;
        wxString username(upd.getUsername());
        wxString password(upd.getPassword());

        try
        {
            dalSvc = fr::DatabaseFactory::createService();
            dalSvc->setConnectionString(wx2std(s->getConnectionString()));
            dalSvc->setCredentials(wx2std(username), wx2std(password));
            dalSvc->connect();

            // exception might be thrown. If not, we store the credentials:
            if (sysdba || username.Upper() == "SYSDBA")
                s->setServiceSysdbaPassword(password);
            else
                s->setServiceCredentials(username, password);
        }
        catch(const std::exception& e)
        {
            wxMessageBox(wxString::FromUTF8(e.what()), _("Error"),
                wxICON_ERROR | wxOK);
            return false;
        }
    }

    if (auto ibppSvc = std::dynamic_pointer_cast<fr::IbppService>(dalSvc))
    {
        svc = ibppSvc->getIBPPService();
        return true;
    }
    return false;
}

wxString unquote(const wxString& input, const wxString& quoteChar)
{
    wxString result = input;

    if (result.StartsWith(quoteChar) && result.EndsWith(quoteChar) && result.length() >= 2) {
        result = result.Mid(1, result.length() - 2);
    }

    return result;
}

wxString getClientLibrary()
{
    /*Todo: Implement FB library per conexion */
#if defined(_WIN64)
    return config().get("x64LibraryFile", wxString(""));
#else
    return config().get("x86LibraryFile", wxString(""));
#endif

}
