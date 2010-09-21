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

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <sstream>
#include <iomanip>
#include <vector>

#include "config/Config.h"
#include "core/StringUtils.h"
#include "frutils.h"
#include "gui/ProgressDialog.h"
#include "metadata/CreateDDLVisitor.h"
#include "metadata/metadataitem.h"
#include "metadata/server.h"
#include "sql/SqlTemplateProcessor.h"
//-----------------------------------------------------------------------------
SqlTemplateProcessor::SqlTemplateProcessor(MetadataItem *m,
    std::vector<MetadataItem *> *allowedObjects)
	: TemplateProcessor(m, allowedObjects)
{
}
//-----------------------------------------------------------------------------
void SqlTemplateProcessor::processCommand(wxString cmdName,
	wxString cmdParams, MetadataItem *object,
    wxString& processedText, wxWindow *window, bool first)
{
	TemplateProcessor::processCommand(cmdName, cmdParams, object, processedText, window, first);

	// TODO: It would be better if this was translated to the folder of the
	// actual template we are processing, in order to support templates in
	// different folders.
	if (cmdName == wxT("template_root"))
		processedText += config().getSqlTemplatesPath();
}
//-----------------------------------------------------------------------------
wxString SqlTemplateProcessor::escapeChars(const wxString& input, bool)
{
	return input;
}
//-----------------------------------------------------------------------------