// --*-c++-*--
/*
    $Id: hex_edit.cpp,v 1.1 2004/01/05 21:59:03 thementat Exp $
 
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

using namespace std;
#include "hex_edit.h"

IMPLEMENT_DYNAMIC_CLASS(wxHexEditCtrl, wxScrolledWindow)

BEGIN_EVENT_TABLE(wxHexEditCtrl, wxScrolledWindow)
    EVT_PAINT(wxHexEditCtrl::OnPaint)
    EVT_SIZE(wxHexEditCtrl::OnSize)
    EVT_CHAR(wxHexEditCtrl::OnChar)
    EVT_LEFT_UP(wxHexEditCtrl::OnMouseUp)
    EVT_LEFT_DOWN(wxHexEditCtrl::OnMouseDown)
    EVT_MOTION(wxHexEditCtrl::OnMouseMove)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_HEX_CHANGED)

wxHexEditCtrl::wxHexEditCtrl( wxWindow *parent, wxWindowID id,
        const wxPoint& pos, const wxSize& size, 
        long style, const wxString& name)
        : wxScrolledWindow( parent, id, pos, size, style)
{

    m_indexHigh = true;
    m_currentIndex = 0;
    SetBackgroundColour(*wxWHITE);

    m_xChars = m_yChars = 0;
    m_xMargin = m_yMargin = 5;
        
    SetupFont();
           
}

void wxHexEditCtrl::Home() 
{ 
    m_currentIndex = 0; 
    m_indexHigh = true;
}

void wxHexEditCtrl::End() 
{
    if (m_buffer.Length() == 0)
        return;
        
    m_currentIndex = m_buffer.Length() - 1;
    m_indexHigh = false; 
    Refresh(false);       
}

void wxHexEditCtrl::PrevChar() 
{
    if (m_buffer.Length() == 0)
        return;
        
    if (!m_indexHigh)
        m_indexHigh = true;
    else
    {
        m_indexHigh = false;
        if (m_currentIndex == 0)
            m_currentIndex = m_buffer.Length() - 1;
        else
            m_currentIndex--;
    }
    Refresh(false);
}
 
void wxHexEditCtrl::NextChar() 
{
    if (m_buffer.Length() == 0)
        return;
        
    if (m_indexHigh)
        m_indexHigh = false;
    else
    {
        m_indexHigh = true;
        if (m_currentIndex == (m_buffer.Length() - 1))
            m_currentIndex = 0;
        else
            m_currentIndex++;
    }
    Refresh(false);
}

void wxHexEditCtrl::PrevLine() 
{ 
    if (m_currentIndex > (m_xBytes + 1))
        m_currentIndex -= m_xBytes;
    else 
        return;
    // should wrap but we DONT   
    Refresh(false);    

}

void wxHexEditCtrl::NextLine() 
{
    if (((m_currentIndex + 1) + m_xBytes) < m_buffer.Length())
    {
        m_currentIndex += m_xBytes;
        Refresh(false);
    }
}

void wxHexEditCtrl::CalculateSize(int xSize, int ySize)
{
    bool horzscroll = false, 
        vertscroll = false;

    // total displayable chars
    m_xChars = (xSize - 2*m_xMargin) / m_widthChar;
    m_yChars = (ySize - 2*m_yMargin) / m_heightChar;
     
    // at least MIN_BYTES_PER_LINE per line
    if (m_xChars < (MIN_BYTES_PER_LINE * 3 - 1)) 
    {
        m_xChars = (MIN_BYTES_PER_LINE * 3 - 1);
        horzscroll = true;
        m_xBytes = MIN_BYTES_PER_LINE;   
    }
    else
    {
        if (m_xChars == 2) // the weird case
            m_xBytes = 1;
        else
            m_xBytes = (m_xChars + 1)/3;
    }
    
    unsigned int total = m_xBytes * m_yChars;
    
    if (m_buffer.Length() > total)
        vertscroll = true;

    if (vertscroll && horzscroll)
        SetScrollbars(m_widthChar * 2, m_heightChar, m_xChars, m_yChars + 1, 0, 0, TRUE /* no refresh */);
    else 
        if (vertscroll)
            SetScrollbars(0, m_heightChar, 0, m_yChars + 1, 0, 0, TRUE /* no refresh */); 
    else 
        if (horzscroll)
            SetScrollbars(m_widthChar * 2, 0 , m_xChars, 0, 0, 0, TRUE /* no refresh */);
    else 
        SetScrollbars(0,0,0,0,0,0, TRUE);

}

wxHexEditCtrl::~wxHexEditCtrl()
{

}

void wxHexEditCtrl::SetupFont()
{
    m_font = wxFont(12, wxFONTFAMILY_TELETYPE,
                    wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxClientDC dc(this);
    dc.SetFont(m_font);
    m_heightChar = dc.GetCharHeight();
    m_widthChar = dc.GetCharWidth();

}
/*
void wxHexEditCtrl::PointToCharXY(const int pointX, const int pointY, int& charX, int& charY);
{
    charX =  ((pointX - m_xMargin) / m_widthChar);
    if (charX >= m_xChars) 
        charX = m_xChars - 1;
    
    charY = ((pointY - m_yMargin) / m_heightChar);
    if (charY >= m_yChars) 
        charY = m_yChars - 1;
    
    // check to see if in an invalid area (space in hex view)
    if (((charX + 1) % 3) == 0)
        --charX;
  
}

void wxHexEditCtrl::CharXYToOffset(const unsigned int x, const unsigned int y, 
    unsigned int& off, bool& high_nib)
{
    if (((x + 1) % 3) == 1)
        high_nib = true;
    else
        high_nib = false;
    
    off = (y*m_xChars + x + 1)/3;
    
}
*/
/*
void wxHexEditCtrl::OffsetToCharXY(const int off, int& x, int& y)
{
    if (off == 0)
    {
        y = 0;
        x = 0;
    }
    else
    {
        y = off / m_xBytes;
        if ((off % m_xBytes) == 0)
            x = 0;
        else
            x = off % m_xBytes * 3; 
    }
}*/

unsigned long wxHexEditCtrl::PointToOffset(int pointX, int pointY, bool& high_nib)
{
    unsigned short charX = ((pointX - m_xMargin) / m_widthChar);
    cout << "CharX " << charX << endl;
    unsigned short charY = ((pointY - m_yMargin) / m_heightChar);
    cout << "CharY " << charY << endl;

    unsigned short bytesInLine = (m_xChars + 1)/3;
    
    // check for invalid x
    if ((pointX <= m_xMargin) || (pointX >= (m_xMargin + m_widthChar * (bytesInLine * 3 - 1))))
        return m_currentIndex;
    cout << "Valid X\n";   
    // check for invalid y
    if ((pointY <= m_yMargin) || (pointY >= (m_yMargin + m_heightChar * m_yChars)))
        return m_currentIndex;
    cout << "Valid Y\n";
          
    cout << "Bytes in line: " << bytesInLine << endl;
    
    unsigned int off = (charX + 1) / 3 + bytesInLine * charY;
    
    cout << "Proposed off: " << off << endl;
    
    if (((charX + 1) % 3) == 1)
        high_nib = true;
    else
        high_nib = false;
    
    if (off >= m_buffer.Length())
        return m_currentIndex;
    
    
        
    cout << "Off Valid: 0x" << hex << off << endl;
    
    return off;
       
}

void wxHexEditCtrl::OnMouseMove( wxMouseEvent& event)
{
    if (!event.LeftIsDown())
    {   
        event.Skip();
        return;
    }
    
    // would get highlighting here
    
    m_currentIndex = PointToOffset(event.GetX(), event.GetY(), m_indexHigh);
    Refresh(false);
}

void wxHexEditCtrl::OnMouseDown( wxMouseEvent &event)
{
    m_currentIndex = PointToOffset(event.GetX(), event.GetY(), m_indexHigh);
    Refresh(false);
    wxLogDebug("OnMouseDown"); 
}

void wxHexEditCtrl::OnMouseUp( wxMouseEvent &event)
{

}
/*
bool wxHexEditCtrl::Valid(int x, int y)
{
    bool high;
    unsigned int off, x1 = x, y1 = y;
    
    CharXYToOffset(x1, y1, off, high);
     
    if (off >= m_buffer.Length())
    {
        cout << "Invalid Offset: " << hex << off << " Char x:" << dec << x1 << " y:" << y1 << endl;  
      
        return false;
    }
    cout << "Valid Offset: " << hex << off << " Char x:" << dec << x1 << " y:" << y1 << endl;  
    
    return true;
  
}
*/
void wxHexEditCtrl::ByteToHex(unsigned char c, char out[2])
{
    // code from GHex2, GPL
    unsigned int low, high;
    
    low = c & 0x0F;
    high = (c & 0xF0) >> 4;
        
    out[0] = ((high < 10)?(high + '0'):(high - 10 + 'A'));
    out[1] = ((low < 10)?(low + '0'):(low - 10 + 'A'));  
}

void wxHexEditCtrl::SetIndex(char c, unsigned int index, bool high_nib)
{
    
    if (index >= m_buffer.Length())
    {
        wxLogDebug(wxT("wxHexEditCtrl::SetIndex: index >= buffer size"));
        return;
    }

    unsigned char val;
    
    if ((c >= '0') && (c <= '9'))
        val = (unsigned char)(c - 48);
    if ((c >= 'a') && (c <= 'f'))
        val = (unsigned char)(c - 97) + 10;
    if ((c >= 'A') && (c <='F'))
        val = (unsigned char)(c - 65) + 10;
        
    
    // set high nibble
    if (high_nib)
    {
        cout << "Nib is high: " << hex << (unsigned int)val << endl;
        m_buffer[index] &= 0x0f;
        m_buffer[index] |= ((val << 4) & 0xf0);
    }
    else
    {
        cout << "Nib is low: " << hex << (unsigned int)val << endl;
        m_buffer[index] &= 0xf0;
        m_buffer[index] |= (val & 0x0f);
    }
    Refresh(FALSE);
  
}

void wxHexEditCtrl::OnSize( wxSizeEvent &event )
{
    CalculateSize(event.GetSize().x, event.GetSize().y);

    event.Skip();
}

void wxHexEditCtrl::DrawHighlights(wxDC * dc)
{
  
    wxBrush& oldBrush = dc->GetBrush();
    wxPen& oldPen = dc->GetPen();
    
    dc->SetPen(*wxTRANSPARENT_PEN);
    int cursorX;
    int cursorY;
    
    /// highlights
    for (unsigned int i = 0; i < m_highlights.size(); i++)
    {
        //wxColour hl(m_highlights[i].Red, m_highlights[i].Blue, m_highlights[i].Green);
        wxBrush aBrush(m_highlights[i].Color, wxSOLID);
        dc->SetBrush(aBrush);
        
        int endX;
        int spanY;
        int lastX;
        
        unsigned int bytelength = m_highlights[i].End - m_highlights[i].Start;
        cout << "Length: " << bytelength << endl;
        cursorX = ((m_highlights[i].Start % m_xBytes) * 3) * m_widthChar + m_xMargin;
        cursorY = ((m_highlights[i].Start / m_xBytes)) * m_heightChar + m_yMargin;
        
        cout << "cursorX " << dec << cursorX << endl;
        cout << "cursorY " << cursorY << endl;
        
        if ((m_highlights[i].End % m_xBytes) == 0)
            endX = (m_xBytes * 3 - 1) * m_widthChar;
        else
            endX = ((m_highlights[i].End % m_xBytes) * 3 - 1) * m_widthChar;
        
        spanY = ((m_highlights[i].End / m_xBytes)) * m_heightChar + m_yMargin;
        
        cout << "endX " << endX << endl;
        cout << "spanY " << spanY << endl;
        
        lastX = ((m_xBytes * 3 - 1) * m_widthChar);
        
        cout << "lastX " << lastX << endl;  
      
        //ug, A BIG FAT HACK
        if (cursorY != spanY)
        {
            // render first line
            //int linelength = m_widthChar * (m_xChars - cursorX);
            dc->DrawRectangle(cursorX, cursorY, lastX-cursorX + m_xMargin, m_heightChar);
            // render middle
            unsigned int lines;
            
            if (bytelength > 2*m_xBytes)
                lines = (bytelength - (m_xBytes - (m_highlights[i].Start % m_xBytes))) / m_xBytes;
            else
                lines = 0;
            
            cout << "lines " << lines << endl;
            for (unsigned int i = 1; i <= lines; i++)
            {
                cout << "Drawing: " << m_xMargin << " " << (cursorY + i * m_heightChar)
                    << " " << lastX << " " << m_heightChar << endl;
                dc->DrawRectangle(m_xMargin, cursorY + i * m_heightChar, lastX, m_heightChar);
              
            }
            // render last
            if (endX != lastX)
                dc->DrawRectangle(m_xMargin, spanY, endX, m_heightChar);
            
        }
        else
        {
            dc->DrawRectangle(cursorX, cursorY, endX-cursorX, m_heightChar);    
        }
    }
    
    // cursor
    
    dc->SetBrush(*wxBLUE_BRUSH);
    
    if (m_currentIndex == 0)
        cursorX = m_xMargin;
    else
        cursorX = ((m_currentIndex % m_xBytes) * 3) * m_widthChar + m_xMargin;
    
    if (!m_indexHigh)
        cursorX += m_widthChar;
    
    cursorY = ((m_currentIndex / m_xBytes)) * m_heightChar + m_yMargin;
    
    dc->DrawRectangle(cursorX, cursorY, m_widthChar, m_heightChar);
    
    dc->SetBrush(oldBrush);
    dc->SetPen(oldPen);
  
}

void wxHexEditCtrl::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );
    PrepareDC( dc );
    dc.Clear();
    dc.SetFont( m_font );
    int lines = 0;
    
    DrawHighlights(&dc);
   
    for (unsigned int y = 0; y < m_buffer.Length() ;)
    {
        wxString line;

        for ( unsigned int x = 0; (x < m_xBytes) && (y < m_buffer.Length()); x++ ) // minus one since we want 2 at a time
        {
            char ch[2];
            ByteToHex(m_buffer[y], ch);

            line += ch[0];      
            line += ch[1];
              
            if (x != (m_xBytes - 1))
                line += ' ';
            y++;
        }
        
        dc.DrawText( line, m_xMargin, m_yMargin + lines++ * m_heightChar );
    }
    
}

void wxHexEditCtrl::SetBuffer(const ByteBuffer& buf)
{
    m_buffer = buf;
    Refresh(false);
}

ByteBuffer wxHexEditCtrl::GetBuffer()
{
    return m_buffer;  
}

int wxHexEditCtrl::SetHighlight(Highlight newHL)
{
    // todo: check for overlapping regions
    m_highlights.push_back(newHL);
    return m_highlights.size() - 1;
  
}

wxHexEditCtrl::Highlight 
wxHexEditCtrl::GetHighlight(unsigned int index)
{
      
    if (index >= m_highlights.size())
        return Highlight();
    
    return m_highlights[index];
  
}

bool wxHexEditCtrl::RemoveHighlight(unsigned int index)
{
    return true;
}

void wxHexEditCtrl::OnChar( wxKeyEvent &event )
{
  
    switch ( event.KeyCode() )
    {
        case WXK_LEFT:
            PrevChar();
            break;

        case WXK_RIGHT:
            NextChar();
            break;

        case WXK_UP:
            PrevLine();
            break;

        case WXK_DOWN:
            NextLine();
            break;

        case WXK_HOME:
            Home();
            break;

        case WXK_END:
            End();
            break;

        case WXK_RETURN:
            Home();
            NextLine();
            break;

        default:
        
            wxChar ch = (wxChar)event.KeyCode();
            if ( !event.AltDown() && ( ((ch >= 'a') && (ch <= 'f')) || 
                ((ch >= 'A') && (ch <= 'F'))  || ((ch >= '0') && (ch <= '9')) ) )    
            {

                wxCommandEvent event(wxEVT_HEX_CHANGED, GetId());
                event.SetInt((int)m_currentIndex);
                wxPostEvent(this, event);
                              
                // set the buffer value
                SetIndex(ch, m_currentIndex, m_indexHigh);

                NextChar();
            }
            else
            {
                event.Skip();
            }
    }
    
    //DoMoveCaret();
}
