// --*-c++-*--
/*
    $Id: hex_edit.h,v 1.1 2004/01/05 21:59:06 thementat Exp $
 
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
#ifndef WX_HEX_EDIT_H
#define WX_HEX_EDIT_H

#include "wx/wx.h"

#include "packet.h"

#define MIN_BYTES_PER_LINE 8

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_HEX_CHANGED, 8888)
END_DECLARE_EVENT_TYPES()

// it may also be convenient to define an event table macro for this event type
#define EVT_HEX_CHANGED(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_HEX_CHANGED, id, -1, \
        (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&fn, \
        (wxObject *) NULL \
    ),


class wxHexEditCtrl: public wxScrolledWindow
{
public:

    wxHexEditCtrl() {}
    wxHexEditCtrl( wxWindow *parent, wxWindowID id = -1,
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, 
        long style = wxHSCROLL | wxVSCROLL, 
        const wxString& name = "wxHexEditCtrl" );
    ~wxHexEditCtrl();

    // movement
    void Home();
    void End();
    void PrevChar();
    void NextChar();
    void PrevLine();
    void NextLine();
    
    
    // event handlers
    void OnPaint( wxPaintEvent &event );
    void OnSize( wxSizeEvent &event );
    void OnChar( wxKeyEvent &event );
    void OnMouseDown( wxMouseEvent &event);
    void OnMouseUp( wxMouseEvent &event);
    void OnMouseMove( wxMouseEvent &event);
    
    // Buffer manip
    
    void SetBuffer(const ByteBuffer& buf);
    inline ByteBuffer GetBuffer();
    
    // Control insert    
    void SetInsert(bool can_insert) { m_insert= can_insert; }
    bool CanInsert() const { return m_insert; }
    
    
    struct Highlight
    {
        word32 Start;
        word32 End;
        wxColour Color;
    };
    
    // highlighting control
    /// Returns -1 if highlights overlap or the index of the highlight
    int SetHighlight(Highlight newHL);
    Highlight GetHighlight(unsigned int index);
    bool RemoveHighlight(unsigned int index);
    
    
protected:

    void DrawHighlights(wxDC * dc);
    // if insert is off, can't go past what is in the buffer
    // so returns false
    bool Valid(int charx, int chary);
    
    // operations
    void SetupFont();

    // calculates new bounds of character matrix after resize
    void CalculateSize(int xSize, int ySize);

    // Takes a byte and converts it to a 2 chacter hex value
    void ByteToHex(unsigned char c, char out[2]);
     
    // Set the value at the given index and nib
    void SetIndex(char c, unsigned int index, bool high_nib);

    unsigned long PointToOffset(int xPoint, int yPoint, bool& high_nib);    

private:

    wxFont   m_font;

    // the margin around the text (looks nicer)
    int      m_xMargin, m_yMargin;

    // size (in pixels) of one character
    long     m_widthChar, m_heightChar;
    
    // the size (in text coords) of the window unscrolled
    int      m_xChars, m_yChars;
    
    // the size in bytes (in formatted hex rows)
    unsigned int      m_xBytes;

    // internal buffer
    ByteBuffer m_buffer;
    
    unsigned long m_currentIndex;
    bool m_indexHigh; // true if current index is at high nibble
    
    // insert mode
    bool m_insert;
    
private: // yes, I like to keep saying private for no reason

    
    vector<Highlight> m_highlights;

    DECLARE_DYNAMIC_CLASS(wxHexEditCtrl)
    DECLARE_EVENT_TABLE()
    
};

#endif
