// --*-c++-*--
/*
    $Id: craft.h,v 1.1 2004/01/05 21:59:04 thementat Exp $
 
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
#ifndef CRAFT_H
#define CRAFT_H

#include "wx/wx.h"

#include "ipv4_packet.h"
#include "tcp_packet.h"

class wxHexEditCtrl;
class Crafty : public wxApp
{
public:

    virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:

    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnHexChanged(wxCommandEvent& event);
    void OnSendPacket(wxCommandEvent& event);
    
    void OnTcpOptions(wxCommandEvent& event);
    void OnIpOptions(wxCommandEvent& event);    
    void OnIp(wxCommandEvent& event);
    void OnTcp(wxCommandEvent& event);
    
    /// tcp


private:

    void SetPointers();

private:

    TCPPacket m_packet;


    // control pointers ipv4
    wxTextCtrl * m_ipv4Dest;
    wxTextCtrl * m_ipv4Source;
    wxTextCtrl * m_ipv4ProtocolOther;
    wxTextCtrl * m_ipv4Length;
    
    wxCheckBox * m_ipv4AutoSource;
    wxCheckBox * m_ipv4FlagMF;
    wxCheckBox * m_ipv4FlagLF;
    wxCheckBox * m_ipv4LengthOverride;
    
    wxChoice * m_ipv4Protocol;
    
    wxStaticText * m_ipv4LengthTxt;
    
    wxButton * m_ipv4Options;
    
    wxHexEditCtrl * m_ipHex;
    
    // control pointers tcp
    
    wxTextCtrl * m_tcpSourcePort;
    wxTextCtrl * m_tcpDestPort;
    wxTextCtrl * m_tcpSeq;
    wxTextCtrl * m_tcpAck;
    wxTextCtrl * m_tcpHdrLen;
    wxTextCtrl * m_tcpWinSize;
    wxTextCtrl * m_tcpUrgentPtr;
    wxTextCtrl * m_tcpChecksum;
    
    wxCheckBox * m_tcpSeqRandom;
    wxCheckBox * m_tcpAckRandom;
    wxCheckBox * m_tcpR1;
    wxCheckBox * m_tcpR2;
    wxCheckBox * m_tcpR3;
    wxCheckBox * m_tcpR4;
    wxCheckBox * m_tcpR5;
    wxCheckBox * m_tcpR6;
    wxCheckBox * m_tcpHdrLenOverride;
    wxCheckBox * m_tcpFlagURG;
    wxCheckBox * m_tcpFlagACK;
    wxCheckBox * m_tcpFlagPSH;
    wxCheckBox * m_tcpFlagRST;
    wxCheckBox * m_tcpFlagSYN;
    wxCheckBox * m_tcpFlagFIN;
    wxCheckBox * m_tcpChecksumOverride;
    
    wxButton * m_tcpOptions;
    
    wxStaticText * m_tcpHdrLenTxt;
    wxStaticText * m_tcpPacketTxt;
    
    wxHexEditCtrl * m_tcpHex;  

    DECLARE_EVENT_TABLE()
};

#endif
