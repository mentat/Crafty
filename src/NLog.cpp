// --*-c++-*--
/*
    $Id: NLog.cpp,v 1.1 2004/01/05 21:59:05 thementat Exp $
 
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

#include <iostream>

#include "NLog.h"

#include "wx/log.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/listctrl.h"
#include "wx/bitmap.h"

#include "bitmaps/stormtroop.xpm"
#include "bitmaps/yoda.xpm"
#include "bitmaps/boba_ship.xpm"
#include "bitmaps/dstar.xpm"

using namespace std;

guiLog::guiLog(wxWindow* parent, wxWindowID id, const wxPoint& pos, 
    const wxSize& size, long style, 
    const wxValidator& validator, 
    const wxString& name)
: wxListCtrl(parent, id, pos, size, style, validator, name)
{
 
    
    InsertColumn(0, wxT("Type"));
    InsertColumn(1, wxT("Message"));
    

    // prepare the imagelist
    enum { ICON_SIZE = 16 };
    
    wxImageList *imageList = new wxImageList(ICON_SIZE, ICON_SIZE, true);

    // order should be the same as in the switch below!

    imageList->Add( wxBitmap( stormtroop_xpm ).ConvertToImage().Rescale(ICON_SIZE, ICON_SIZE) );
    imageList->Add( wxBitmap( boba_ship_xpm ).ConvertToImage().Rescale(ICON_SIZE, ICON_SIZE) );
    imageList->Add( wxBitmap( yoda_xpm ).ConvertToImage().Rescale(ICON_SIZE, ICON_SIZE) );
    imageList->Add( wxBitmap( dstar_xpm ).ConvertToImage().Rescale(ICON_SIZE, ICON_SIZE) );
    

    SetImageList(imageList, wxIMAGE_LIST_SMALL);

    Show(true);
    
    delete wxLog::SetActiveTarget(this);
    
}


guiLog::~guiLog()
{
    wxLog::SetActiveTarget(NULL);
	delete GetImageList(wxIMAGE_LIST_SMALL);
}

void guiLog::DoLog(wxLogLevel level, const wxChar *szString, time_t t)
{
  int image;

  wxString string_type;
  cout << "LOG: " << szString << endl;

  switch ( level )
  {
  case wxLOG_Error:
    image = 0;
    string_type = wxT("Error");
    break;
  case wxLOG_Warning:
    image = 1;
    string_type = wxT("Warning");
    break;
  case wxLOG_FatalError:
    image = 0;
    string_type = wxT("Fatal error");
    break;
  case wxLOG_Message:
    image = 2;
    string_type = wxT("Message");
    break;
  case wxLOG_Info:
    image = 2;
    string_type = wxT("Info");
    break;
  case wxLOG_Status:
    image = 2;
    string_type = wxT("Status");
    break;
  case wxLOG_Debug:
    image = 2;
    string_type = wxT("Debug");
    break;
  case wxLOG_Trace:
    image = 2;
    string_type = wxT("Trace");
    break;
  case wxLOG_Progress:
    image = 2;
    string_type = wxT("Progress");
    break;
  case wxLOG_PACKET_IN:
    image = 2;
    string_type = wxT("Packet In");
    break;
  case wxLOG_PACKET_OUT:
    image = 2;
    string_type = wxT("Packet Out");
    break;
  case wxLOG_BUG:
    image = 3;
    string_type = wxT("BUG");
    break;
  default:
    string_type = wxT("Unknown");
    image = 2;
    break;
  }

  InsertItem(0, string_type, image);
  SetItem(0,1,szString);

  SetColumnWidth(0, wxLIST_AUTOSIZE);
  SetColumnWidth(1, wxLIST_AUTOSIZE);

}

void wxLogPacketIn(const wxChar* sxFormat,  ...)
{
    va_list argptr;
    va_start(argptr, sxFormat);
    wxLogGeneric(wxLOG_PACKET_IN, sxFormat, argptr);
    va_end(argptr);
    
}

void wxLogPacketOut(const wxChar* sxFormat,  ...)
{
    va_list argptr;
    va_start(argptr, sxFormat);
    wxLogGeneric(wxLOG_PACKET_OUT, sxFormat, argptr);
    va_end(argptr);
    
}

void wxLogBug(const wxChar* sxFormat, ...)
{
#ifdef DEBUG
    va_list argptr;
    va_start(argptr, sxFormat);
    wxLogGeneric(wxLOG_BUG, sxFormat, argptr);
    va_end(argptr);
#endif
}

/*
    -----
    $Log: NLog.cpp,v $
    Revision 1.1  2004/01/05 21:59:05  thementat
    Initial revision

    Revision 1.2  2003/04/13 18:41:16  mentat
    Adding util and removing newtoc.

    Revision 1.1  2003/03/16 02:27:23  mentat
    Import of new module.

    Revision 1.3  2002/11/03 01:53:35  mentat
    Major updates and rewrite.

*/
