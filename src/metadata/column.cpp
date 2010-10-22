/*
  Copyright (c) 2004-2010 The FlameRobin Development Team

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


  $Id$

*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <sstream>

#include "collection.h"
#include "column.h"
#include "config/Config.h"
#include "constraints.h"
#include "database.h"
#include "domain.h"
#include "MetadataItemVisitor.h"
//-----------------------------------------------------------------------------
Column::Column(MetadataItem* parent, const wxString& name)
    : MetadataItem(ntColumn, parent, name)
{
}
//-----------------------------------------------------------------------------
void Column::initialize(bool notnull, wxString source, wxString computedSource,
    wxString collation, wxString defaultValue, bool hasDefault)
{
    bool changed = false;
    if (notnullM != notnull)
    {
        notnullM = notnull;
        changed = true;
    }
    source = source.Strip(wxString::both);
    if (sourceM != source)
    {
        sourceM = source;
        changed = true;
    }
    if (computedSourceM != computedSource)
    {
        computedSourceM = computedSource;
        changed = true;
    }
    collation = collation.Strip(wxString::both);
    if (collationM != collation)
    {
        collationM = collation.Strip(wxString::both);
        changed = true;
    }
    if (defaultM != defaultValue)
    {
        defaultM = defaultValue;
        changed = true;
    }
    if (hasDefaultM != hasDefault)
    {
        hasDefaultM = hasDefault;
        changed = true;
    }
    if (changed)
        notifyObservers();
}
//-----------------------------------------------------------------------------
bool Column::isNullable(bool checkDomain) const
{
    if (notnullM)
        return false;
    if (!checkDomain)
        return true;
    if (Domain* d = getDomain())
        return d->isNullable();
    return true;
}
//-----------------------------------------------------------------------------
bool Column::hasDefault(bool checkDomain) const
{
    if (hasDefaultM)
        return true;
    if (!checkDomain)
        return true;
    if (Domain* d = getDomain())
        return d->hasDefault();
    return false;
}
//-----------------------------------------------------------------------------
bool Column::isPrimaryKey() const
{
    Table* t = getTable();
    if (!t) // view/SP
        return false;
    ColumnConstraint *key = t->getPrimaryKey();
    if (!key)
        return false;
    for (ColumnConstraint::const_iterator it = key->begin(); it != key->end(); 
        ++it)
    {
        if ((*it) == getName_())
            return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
bool Column::isForeignKey() const
{
    Table* t = getTable();
    if (!t) // view/SP
        return false;
    std::vector<ForeignKey> *fks = t->getForeignKeys();
    if (!fks)
        return false;
    for (std::vector<ForeignKey>::iterator it = fks->begin();
        it != fks->end(); ++it)
    {
        for (std::vector<wxString>::const_iterator c2 = (*it).begin();
            c2 != (*it).end(); ++c2)
        {
            if ((*c2) == getName_())
                return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------------------
//! retrieve datatype from domain if possible
wxString Column::getDatatype(bool useConfig)
{
    enum
    {
        showType = 0,
        showFormula,
        showAll
    };
    int flag = (useConfig ? showFormula : showType);
    if (useConfig)
        config().getValue(wxT("ShowComputed"), flag);
    // view columns are all computed and have their source empty
    if (flag == showFormula && !computedSourceM.empty())
        return computedSourceM;

    wxString ret;
    Domain* d = getDomain();
    wxString datatype(d ? d->getDatatypeAsString() : sourceM);

    enum
    {
        showDatatype = 0,
        showDomain,
        showBoth
    };
    int show = (useConfig ? showBoth : showDatatype);
    if (useConfig)
        config().getValue(wxT("ShowDomains"), show);

    if (!d || d->isSystem() || show == showBoth || show == showDatatype)
        ret += datatype;

    if (d && !d->isSystem() && (show == showBoth || show == showDomain))
    {
        if (!ret.empty())
            ret += wxT(" ");
        ret += wxT("(") + d->getName_() + wxT(")");
    }

    if (flag == showAll && !computedSourceM.empty())
        ret += wxT(" (") + computedSourceM + wxT(")");
    return ret;
}
//-----------------------------------------------------------------------------
bool Column::isString() const
{
    Domain *d = getDomain();
    return (d ? d->isString() : false);
}
//-----------------------------------------------------------------------------
Domain* Column::getDomain() const
{
    Database* db = findDatabase();
    if (!db)
        return 0;

    DomainsPtr ds(db->getDomains());
    if (Domain* d = ds->findByName(sourceM))
        return d;
    // since we haven't found the domain, check the database
    return db->loadMissingDomain(sourceM);
}
//-----------------------------------------------------------------------------
Table* Column::getTable() const
{
    return dynamic_cast<Table*>(getParentObjectOfType(ntTable));
}
//-----------------------------------------------------------------------------
wxString Column::getComputedSource() const
{
    return computedSourceM;
}
//-----------------------------------------------------------------------------
wxString Column::getSource() const
{
    return sourceM;
}
//-----------------------------------------------------------------------------
wxString Column::getCollation() const
{
    return collationM;
}
//-----------------------------------------------------------------------------
wxString Column::getDefault() const
{
    if (defaultM.IsEmpty())
    {
        Domain *d = getDomain();
        if (d)
            return d->getDefault();
    }
    return defaultM;
}
//-----------------------------------------------------------------------------
const wxString Column::getTypeName() const
{
    return wxT("COLUMN");
}
//-----------------------------------------------------------------------------
wxString Column::getDropSqlStatement() const
{
    return wxT("ALTER TABLE ") + getTable()->getQuotedName() + wxT(" DROP ") + getQuotedName();
}
//-----------------------------------------------------------------------------
void Column::acceptVisitor(MetadataItemVisitor* visitor)
{
    visitor->visitColumn(*this);
}
//-----------------------------------------------------------------------------
