// --*-c++-*--
/*
    $Id: craft.cpp,v 1.2 2004/01/08 04:09:45 thementat Exp $
 
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
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "wx/wx.h"
#include "wx/socket.h"

#include "packet.h"
#include "craft_wdr.h"
#include "hex_edit.h"
#include "craft.h"
#include "rawsocket.h"
#include "NLog.h"

#include "mondrian.xpm"

using namespace std;


/// Convert a signed integer to a string
string iToA(long candidate)
{
    stringstream tmp;
    tmp << candidate;
    return tmp.str();
}

string iToAX(long candidate)
{
    stringstream tmp;
    tmp << hex << candidate;
    return tmp.str();
}

word32 ipv4AddrToWord(const wxString& addr)
{
    word32 addrint = 0;
    // sanity check, sorry you cant do hex
    for (unsigned int i = 0; i < addr.Length(); i++)
    {   
       wxChar c = addr[i];
       if (!((c >= '0') && (c <= '9') || (c == '.')))
            return 0;  
      
    }
    
    word32 octet; 
    wxString sofar = addr;
    for (unsigned int i = 0; i < 4; i++)
    {
        if (!sofar.BeforeFirst('.').ToULong(&octet))
            return 0;
            
        sofar = sofar.AfterFirst('.');
          
        addrint |= (octet << i*8);
    }
      
    return addrint; 

}

enum
{
    // menu items
    Caret_Quit = 1,
    Caret_About,
    Caret_SetBlinkTime,
    Caret_Move,

    // controls start here (the numbers are, of course, arbitrary)
    Caret_Text = 1000
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(Caret_Quit,  MainFrame::OnQuit)
    EVT_MENU(Caret_About, MainFrame::OnAbout)
    EVT_HEX_CHANGED(ID_IPV4_HEX, MainFrame::OnHexChanged)
    // buttons
    EVT_BUTTON(ID_TCP_OPTIONS_EDIT, MainFrame::OnTcpOptions)
    EVT_BUTTON(ID_IPV4_EDIT_OPTIONS, MainFrame::OnIpOptions)
    EVT_BUTTON(ID_SEND_PACKET, MainFrame::OnSendPacket)

    // ip
    EVT_TEXT(ID_IPV4_DEST, MainFrame::OnIp)
    EVT_TEXT(ID_IPV4_SOURCE, MainFrame::OnIp)
    EVT_TEXT(ID_IPV4_LENGTH, MainFrame::OnIp)
    EVT_TEXT(ID_IPV4_PROTO_OTHER, MainFrame::OnIp)
    EVT_CHECKBOX(ID_IPV4_AUTO_SOURCE, MainFrame::OnIp)
    EVT_CHECKBOX(ID_IPV4_FLAG_MF, MainFrame::OnIp)
    EVT_CHECKBOX(ID_IPV4_FLAG_LF, MainFrame::OnIp)
    EVT_CHECKBOX(ID_IPV4_OVERRIDE_LENGTH, MainFrame::OnIp)
    EVT_CHOICE(ID_IPV4_PROTOCOLS, MainFrame::OnIp)
    
    // tcp
    
    
END_EVENT_TABLE()

IMPLEMENT_APP(Crafty)

bool Crafty::OnInit()
{
    // create and show the main application window
    MainFrame *frame = new MainFrame(_T("Crafty"),
                                 wxPoint(50, 50), wxSize(450, 340));

    frame->Show(TRUE);

    return TRUE;
}


MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

    // create a menu bar
    wxMenu *menuFile = new wxMenu;

    menuFile->Append(Caret_About, _T("&About...\tCtrl-A"), _T("Show about dialog"));
    menuFile->AppendSeparator();
    menuFile->Append(Caret_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _T("&File"));

    SetMenuBar(menuBar);

    wxPanel * panel = new wxPanel(this);
     
    CreateStatusBar(2); 
     
    wxSizer * sizer = Main(panel);
    sizer->SetSizeHints(this);
    SetSize(sizer->GetMinSize());

    // create a status bar just for fun (by default with 1 pane only)
    
    SetStatusText(_T("Crafty"));
       
    // load initial
    ByteBuffer packet = m_packet.Craft();
    //ByteBuffer payload = m_packet.GetPayload();

    SetPointers();
    
    // update hex views
    m_ipHex->SetBuffer(packet);
    //m_tcpHex->SetBuffer(payload);
    
    // color IP yellow and TCP Green need to update when changes made
    
    wxHexEditCtrl::Highlight ipHL, tcpHL;
    ipHL.Start = 0;
    ipHL.End = m_packet.IPLength();
    ipHL.Color = *wxTheColourDatabase->FindColour("YELLOW");
    
    tcpHL.Start = m_packet.IPLength();
    tcpHL.End = m_packet.Length();
    tcpHL.Color = *wxGREEN;
    
    m_ipHex->SetHighlight(ipHL);
    m_ipHex->SetHighlight(tcpHL);
    
   
    // update checksums
    
    m_tcpChecksum->SetValue(iToAX(m_packet.GetTCPChecksum()).c_str());
        
}

void MainFrame::SetPointers()
{
  
     // set pointers
    //m_tcpHex = (wxHexEditCtrl *)FindWindow(ID_TCP_HEX);
    m_ipHex = (wxHexEditCtrl *)FindWindow(ID_IPV4_HEX);
    
    
    m_ipv4Dest = (wxTextCtrl *)FindWindow(ID_IPV4_DEST);
    m_ipv4Source = (wxTextCtrl *)FindWindow(ID_IPV4_SOURCE);
    m_ipv4ProtocolOther = (wxTextCtrl *)FindWindow(ID_IPV4_PROTO_OTHER);
    m_ipv4Length = (wxTextCtrl *)FindWindow(ID_IPV4_LENGTH);
    
    m_ipv4AutoSource = (wxCheckBox *)FindWindow(ID_IPV4_AUTO_SOURCE);
    m_ipv4FlagMF = (wxCheckBox *)FindWindow(ID_IPV4_FLAG_MF);
    m_ipv4FlagLF = (wxCheckBox *)FindWindow(ID_IPV4_FLAG_LF);
    m_ipv4LengthOverride = (wxCheckBox *)FindWindow(ID_IPV4_OVERRIDE_LENGTH);
    
    m_ipv4Protocol = (wxChoice * )FindWindow(ID_IPV4_PROTOCOLS);
    
    m_ipv4LengthTxt = (wxStaticText *)FindWindow(ID_IPV4_LENGTH_TXT);
    
    // control pointers tcp
    
    m_tcpSourcePort = (wxTextCtrl *)FindWindow(ID_TCP_SOURCE);
    m_tcpDestPort = (wxTextCtrl *)FindWindow(ID_TCP_DEST);
    m_tcpSeq = (wxTextCtrl *)FindWindow(ID_TCP_SEQ);
    m_tcpAck = (wxTextCtrl *)FindWindow(ID_TCP_ACK);
    m_tcpHdrLen = (wxTextCtrl *)FindWindow(ID_TCP_HDR_LEN);
    m_tcpWinSize = (wxTextCtrl *)FindWindow(ID_TCP_WINDOW_SIZE);
    m_tcpUrgentPtr = (wxTextCtrl *)FindWindow(ID_TCP_URG_PTR);
    m_tcpChecksum = (wxTextCtrl *)FindWindow(ID_TCP_CHECKSUM);
    
    m_tcpSeqRandom = (wxCheckBox *)FindWindow(ID_TCP_SEQ_RANDOM);
    m_tcpAckRandom = (wxCheckBox *)FindWindow(ID_TCP_ACK_RANDOM);
    m_tcpR1 = (wxCheckBox *)FindWindow(ID_TCP_R1);
    m_tcpR2 = (wxCheckBox *)FindWindow(ID_TCP_R2);
    m_tcpR3 = (wxCheckBox *)FindWindow(ID_TCP_R3);
    m_tcpR4 = (wxCheckBox *)FindWindow(ID_TCP_R4);
    m_tcpR5 = (wxCheckBox *)FindWindow(ID_TCP_R5);
    m_tcpR6 = (wxCheckBox *)FindWindow(ID_TCP_R6);
    m_tcpHdrLenOverride = (wxCheckBox *)FindWindow(ID_TCP_HDR_LEN_OVERRIDE);
    m_tcpFlagURG = (wxCheckBox *)FindWindow(ID_TCP_FLAG_URG);
    m_tcpFlagACK = (wxCheckBox *)FindWindow(ID_TCP_FLAG_ACK);
    m_tcpFlagPSH = (wxCheckBox *)FindWindow(ID_TCP_FLAG_PSH);
    m_tcpFlagRST = (wxCheckBox *)FindWindow(ID_TCP_FLAG_RST);
    m_tcpFlagSYN = (wxCheckBox *)FindWindow(ID_TCP_FLAG_SYN);
    m_tcpFlagFIN = (wxCheckBox *)FindWindow(ID_TCP_FLAG_FIN);
    m_tcpChecksumOverride = (wxCheckBox *)FindWindow(ID_TCP_CHECKSUM_OVERRIDE);
    
    m_tcpHdrLenTxt = (wxStaticText *)FindWindow(ID_TCP_HDR_LEN_TEXT);
    //m_tcpPacketTxt = (wxStaticText *)FindWindow(ID_TCP_PACKET_TEXT);
  
}


// event handlers

void MainFrame::OnSendPacket(wxCommandEvent& event)
{
  
    ByteBuffer packet = m_packet.Craft();
    /*
    RawSocket * sock = new RawSocket();
    
    if (!sock->IsOK())
         wxLogBug(wxT("Can't open socket"));
    else
    {
        if (!sock->Send(packet))
             wxLogBug(wxT("Socket says no!"));
    }
   
    delete sock;
    */
    wxLogBug(wxT("OnSendPacket"));
  
}

void MainFrame::OnTcpOptions(wxCommandEvent& event)
{
    wxLogBug(wxT("OnTcpOptions"));
}

void MainFrame::OnIpOptions(wxCommandEvent& event)
{
     wxLogBug(wxT("OnIPOptions"));
}

void MainFrame::OnHexChanged(wxCommandEvent& event)
{
     wxLogBug(wxT("IP Hex changed at: %ld"), event.GetInt());
  
  
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Crafty"),
                 _T("About Crafty"), wxOK | wxICON_INFORMATION, this);
}


void MainFrame::OnIp(wxCommandEvent& event)
{
    int id = event.GetId();
    
     wxLogBug(wxT("ID is: %d"), id);
    
    switch (id)
    {
        case(ID_IPV4_DEST): {
            word32 addr = ipv4AddrToWord(m_ipv4Dest->GetValue()); 
            m_packet.SetDestAddress(addr);
            break;
        }
        case(ID_IPV4_SOURCE):{
            word32 addr = ipv4AddrToWord(m_ipv4Source->GetValue()); 
            m_packet.SetSourceAddress(addr);
            break;
        }
        case(ID_IPV4_AUTO_SOURCE): break;
        case(ID_IPV4_FLAG_MF): 
            m_packet.SetMF(m_ipv4FlagMF->GetValue()); 
            break;
        case(ID_IPV4_FLAG_LF): 
            m_packet.SetLF(m_ipv4FlagLF->GetValue());
            break;
        case(ID_IPV4_PROTOCOLS): {
            switch (m_ipv4Protocol->GetSelection())
            { /*
             wxT("TCP"), 
        wxT("UDP"), 
        wxT("ICMP"), 
        wxT("IGMP"), 
        wxT("IPinIP"), 
        wxT("Other")
            */
                case(-1): break;
                case(0): m_packet.SetProtocol(0x06); break;
                case(1): m_packet.SetProtocol(0x11); break;
                case(2): m_packet.SetProtocol(0x01); break;
                case(3): m_packet.SetProtocol(0x02); break;
                case(4): m_packet.SetProtocol(0x04); break;
                
                case(5): {
                    word32 proto;
                    if (m_ipv4ProtocolOther->GetValue().ToULong(&proto))
                        m_packet.SetProtocol((byte)proto); 
                    break;
                }
            }
            
            break;
        }
        case(ID_IPV4_PROTO_OTHER): 
        {  
            if (m_ipv4Protocol->GetSelection() == 5)
            {
                word32 proto;
                if (m_ipv4ProtocolOther->GetValue().ToULong(&proto))
                    m_packet.SetProtocol((byte)proto); 
                break;
            }
            break;
        }
        case(ID_IPV4_LENGTH_TXT): break;
        case(ID_IPV4_LENGTH):
        case(ID_IPV4_OVERRIDE_LENGTH): 
        {
            if (m_ipv4LengthOverride->GetValue())
            {
                word32 proto;
                if (m_ipv4Length->GetValue().ToULong(&proto))
                    m_packet.SetProtocol((byte)proto); 
                break;
            }
            break;
        }
        case(ID_IPV4_EDIT_OPTIONS): break;
    }
    
    ByteBuffer packet = m_packet.Craft();

    // update hex views
    m_ipHex->SetBuffer(packet);
    
  
}

void MainFrame::OnTcp(wxCommandEvent& event)
{
  
}
