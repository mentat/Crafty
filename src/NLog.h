// --*-c++-*--
/*
    $Id: NLog.h,v 1.1 2004/01/05 21:59:04 thementat Exp $
 
    Crafty Packet Crafter
    Copyright (C) 2003  Jesse Lovelace <jllovela@ncsu.edu>
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef CRAFTY_LOG_H
#define CRAFTY_LOG_H

#include "wx/wx.h"
#include "wx/log.h"
#include "wx/listctrl.h"

enum
{
    wxLOG_PACKET_OUT = 101,
    wxLOG_PACKET_IN,
    wxLOG_BUG
};

// Yes! Fuck you MI haters.
class guiLog: public wxListCtrl , public wxLog
{
public:
    guiLog(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, long style = wxLC_LIST, 
        const wxValidator& validator = wxDefaultValidator, 
        const wxString& name = "guiLog");
    
    ~guiLog();

protected:

    virtual void DoLog(wxLogLevel level, const wxChar *szString, time_t t);

};

DECLARE_LOG_FUNCTION(PacketIn);
DECLARE_LOG_FUNCTION(PacketOut);
DECLARE_LOG_FUNCTION(Bug);

#endif

/*
enum
{
    wxLOG_FatalError, // program can't continue, abort immediately
    wxLOG_Error,      // a serious error, user must be informed about it
    wxLOG_Warning,    // user is normally informed about it but may be ignored
    wxLOG_Message,    // normal message (i.e. normal output of a non GUI app)
    wxLOG_Info,       // informational message (a.k.a. 'Verbose')
    wxLOG_Status,     // informational: might go to the status line of GUI app
    wxLOG_Debug,      // never shown to the user, disabled in release mode
    wxLOG_Trace,      // trace messages are also only enabled in debug mode
    wxLOG_Progress,   // used for progress indicator (not yet)
    wxLOG_User = 100  // user defined levels start here
};
*/

