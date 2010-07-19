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

#if defined(__WXMSW__)
    #include "wx/msw/wrapwin.h" // for "windows.h"
#endif

#include "config/Config.h"
#include "framemanager.h"
#include "gui/BaseFrame.h"
//-----------------------------------------------------------------------------
BaseFrame::FrameIdMap BaseFrame::frameIdsM;
//-----------------------------------------------------------------------------
BaseFrame::BaseFrame(wxWindow* parent, int id, const wxString& title,
        const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxFrame(parent, id, title, pos, size, style | wxNO_FULL_REPAINT_ON_RESIZE, name)
{
    frameIdsM.insert(FrameIdPair(this, wxEmptyString));
}
//-----------------------------------------------------------------------------
BaseFrame::~BaseFrame()
{
    frameIdsM.erase(this);
}
//-----------------------------------------------------------------------------
bool BaseFrame::Show(bool show)
{
    if (show && !IsShown())
         readConfigSettings();
    return wxFrame::Show(show);
}
//-----------------------------------------------------------------------------
bool BaseFrame::Destroy()
{
    writeConfigSettings();
    return wxFrame::Destroy();
}
//-----------------------------------------------------------------------------
void BaseFrame::readConfigSettings()
{
    // load position and size from config; it values are not set, they will be untouched
    wxRect r = getDefaultRect();
    bool enabled = false;
    bool maximized = false;
    if (config().getValue(wxT("FrameStorage"), enabled) && enabled)
    {
        wxString itemPrefix = getStorageName();
        if (!itemPrefix.empty())
        {
            config().getValue(itemPrefix + Config::pathSeparator + wxT("maximized"), maximized);
            config().getValue(itemPrefix + Config::pathSeparator + wxT("x"), r.x);
            config().getValue(itemPrefix + Config::pathSeparator + wxT("y"), r.y);
            config().getValue(itemPrefix + Config::pathSeparator + wxT("width"), r.width);
            config().getValue(itemPrefix + Config::pathSeparator + wxT("height"), r.height);
            doReadConfigSettings(itemPrefix);
        }
    }
    SetSize(r);
    if (maximized)
        Maximize();
}
void BaseFrame::doReadConfigSettings(const wxString& WXUNUSED(prefix))
{
}
//-----------------------------------------------------------------------------
void BaseFrame::writeConfigSettings() const
{
    // wxFileConfig::Flush() should only be called once
    SubjectLocker locker(&config());

    // propagate call to children frames.
    const wxWindowList& children = GetChildren();
    for (wxWindowList::const_iterator it = children.begin();
        it != children.end(); ++it)
    {
        BaseFrame *f = dynamic_cast<BaseFrame *>(*it);
        if (f)
            f->writeConfigSettings();
    }

    if (config().get(wxT("FrameStorage"), false) && !IsIconized())    // don't save for minimized windows
    {
        // save window position and size to config.
        wxString itemPrefix = getStorageName();
        if (!itemPrefix.empty())
        {
            wxRect r = GetRect();
            // TODO: move this to a better place when source is refactored
#ifdef __WIN32__
            WINDOWPLACEMENT wp;
            wp.length = sizeof(WINDOWPLACEMENT);
            if (::GetWindowPlacement((HWND)GetHandle(), &wp) && IsMaximized())
            {
                r.SetLeft(wp.rcNormalPosition.left);
                r.SetTop(wp.rcNormalPosition.top);
                r.SetRight(wp.rcNormalPosition.right);
                r.SetBottom(wp.rcNormalPosition.bottom);
            }
            config().setValue(itemPrefix + Config::pathSeparator + wxT("maximized"), IsMaximized());
#endif
            config().setValue(itemPrefix + Config::pathSeparator + wxT("x"), r.x);
            config().setValue(itemPrefix + Config::pathSeparator + wxT("y"), r.y);
            config().setValue(itemPrefix + Config::pathSeparator + wxT("width"), r.width);
            config().setValue(itemPrefix + Config::pathSeparator + wxT("height"), r.height);
            doWriteConfigSettings(itemPrefix);
        }
    }
}
//-----------------------------------------------------------------------------
void BaseFrame::doWriteConfigSettings(const wxString& WXUNUSED(prefix)) const
{
}
//-----------------------------------------------------------------------------
const wxString BaseFrame::getName() const
{
    // Couldn't find a reliable (meaning supportable and cross-platform) way
    // to use the class name here, so every derived frame needs to override getName()
    // if it needs to use features that depend on it.
    return wxT("");
}
//-----------------------------------------------------------------------------
const wxString BaseFrame::getStorageName() const
{
    return getName();
}
//-----------------------------------------------------------------------------
const wxRect BaseFrame::getDefaultRect() const
{
    return wxRect(wxDefaultPosition, wxDefaultSize);
}
//-----------------------------------------------------------------------------
void BaseFrame::setIdString(BaseFrame* frame, const wxString& id)
{
    FrameIdMap::iterator it = frameIdsM.find(frame);
    if (it != frameIdsM.end())
        (*it).second = id;
}
//-----------------------------------------------------------------------------
BaseFrame* BaseFrame::frameFromIdString(const wxString& id)
{
    if (!id.IsEmpty())
    {
        FrameIdMap::iterator it;
        for (it = frameIdsM.begin(); it != frameIdsM.end(); it++)
        {
            if ((*it).second == id)
                return (*it).first;
        }
    }
    return 0;
}
//-----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(BaseFrame, wxFrame)
    EVT_CLOSE(BaseFrame::OnClose)
END_EVENT_TABLE()
//-----------------------------------------------------------------------------
void BaseFrame::OnClose(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}
//-----------------------------------------------------------------------------
