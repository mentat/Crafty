// --*-c++-*--
/*
    $Id: tcp_packet.cpp,v 1.1 2004/01/05 21:59:06 thementat Exp $
 
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
#include "cryptopp/misc.h"

#include "tcp_packet.h"

using namespace std;
using namespace CryptoPP;

TCPPacket::TCPPacket():
    m_sourcePort(0), m_destPort(0), m_sequence(0), m_ackNumber(0),
    m_headerLength(0), m_reserved(0), m_flags(0), m_windowSize(0),
    m_tcpChecksum(0), m_urgentPointer(0)
{
    m_bypassChecksum = false;
    m_bypassHdrLength = false;

    SetProtocol(0x06);
}

unsigned int TCPPacket::Length()
{
    return IPv4Packet::Length() + TCPLength();
}

word16 TCPPacket::GetTCPChecksum() const
{
    return m_tcpChecksum;
  
}


word16 TCPPacket::TCPLength()
{
    return BASE_TCP_HEADER_LENGTH + m_options.Length() + m_payload.Length();
}
    
ByteBuffer TCPPacket::Craft()
{
    ByteBuffer dummy;
    return DoCraft(dummy);  
  
}

ByteBuffer TCPPacket::DoCraft(ByteBuffer& buff)
{
    ByteBuffer tcp(TCPLength());
 
    tcp[0] = (byte)(m_sourcePort & 0xff);
    tcp[1] = (byte)((m_sourcePort >> 8) & 0xff);
    
    tcp[2] = (byte)(m_destPort & 0xff);
    tcp[3] = (byte)((m_destPort >> 8) & 0xff);

    // sequence
    tcp[4] = (byte)(m_sequence & 0xff);
    tcp[5] = (byte)((m_sequence >> 8) & 0xff);
    tcp[6] = (byte)((m_sequence >> 16) & 0xff);
    tcp[7] = (byte)((m_sequence >> 24) & 0xff);
    
    // ack #
    tcp[8] = (byte)(m_ackNumber & 0xff);
    tcp[9] = (byte)((m_ackNumber >> 8) & 0xff);
    tcp[10] = (byte)((m_ackNumber >> 16) & 0xff);
    tcp[11] = (byte)((m_ackNumber >> 24) & 0xff);
    
    if (!m_bypassHdrLength)
    {   // todo: make sure this is right
        m_headerLength = bitset<4>((BASE_TCP_HEADER_LENGTH + m_options.Length())/4);
    }
    
    tcp[12] = (byte)((m_headerLength.to_ulong() << 4) & 0xf0);
    tcp[12] |= (byte)((m_reserved.to_ulong() >> 2) & 0x0f);
    
    tcp[13] = (byte)((m_reserved.to_ulong() << 4) & 0xc0);
    tcp[13] |= (byte)(m_flags.to_ulong() & 0x3f);
    
    tcp[14] = (byte)(m_windowSize & 0xff);
    tcp[15] = (byte)((m_windowSize >> 8) & 0xff);
    
    
    // if we dont bypass the checksum, set to 0 here and calculate at the end
    // with psuedo header, blah.
    if (!m_bypassChecksum)
        m_tcpChecksum = 0;
       
    tcp[16] = (byte)(m_tcpChecksum & 0xff);
    tcp[17] = (byte)((m_tcpChecksum >> 8) & 0xff);
    
    tcp[18] = (byte)(m_urgentPointer & 0xff);
    tcp[19] = (byte)((m_urgentPointer >> 8) & 0xff);
    
    // options
    //tcp.insert(tcp.end(), m_options.begin(), m_options.end());
    tcp.CopyFrom(20, m_options.Length(), m_options.Get());
    
    // payload
    
    //tcp.insert(tcp.end(), m_payload.begin(), m_payload.end());
    tcp.CopyFrom(20 + m_options.Length(), m_payload.Length(), m_payload.Get());
    
    if (!m_bypassChecksum)
    {
        // calculate checksum
        
        m_tcpChecksum = CalculateChecksum(tcp);
        tcp[16] = (byte)(m_tcpChecksum & 0xff);
        tcp[17] = (byte)((m_tcpChecksum >> 8) & 0xff);
        cout << "Calc checksum: " << hex << m_tcpChecksum << endl;
        cout << "16: " << hex << (int)tcp[16] << " 17: " << (int)tcp[17] << endl;
      
    }
    
    return IPv4Packet::DoCraft(tcp);      
  
}

ByteBuffer TCPPacket::GetPayload()
{
    return m_payload;
}


word16 TCPPacket::CalculateChecksum(ByteBuffer& buff, word16 overide)
{
    bool pad;
    word16 length;
    
    if (buff.Length() % 2 == 1)
        pad = true;
    else
        pad = false;
        
    if (pad)
        length = buff.Length() + 13;
    else
        length = buff.Length() + 12;
        
    ByteBuffer fakePacket(length);
    
    // fill in fake ass header
    fakePacket[0] = (byte) (IPv4Packet::GetSourceAddress() & 0xff);
    fakePacket[1] = (byte) ((IPv4Packet::GetSourceAddress() >> 8) & 0xff);
    fakePacket[2] = (byte) ((IPv4Packet::GetSourceAddress() >> 16) & 0xff);
    fakePacket[3] = (byte) ((IPv4Packet::GetSourceAddress() >> 24) & 0xff);
    fakePacket[4] = (byte) (IPv4Packet::GetDestAddress() & 0xff);
    fakePacket[5] = (byte) ((IPv4Packet::GetDestAddress() >> 8) & 0xff);
    fakePacket[6] = (byte) ((IPv4Packet::GetDestAddress() >> 16) & 0xff);
    fakePacket[7] = (byte) ((IPv4Packet::GetDestAddress() >> 24) & 0xff);
    fakePacket[8] = (byte)0;
    fakePacket[9] = IPv4Packet::GetProtocol();
    fakePacket[10] = (NBO(TCPLength()) & 0xff);
    fakePacket[11] = ((NBO(TCPLength()) >> 8) & 0xff);
    
    fakePacket.CopyFrom(12, buff.Length(), buff.Get());
    
    if (pad)
        fakePacket[fakePacket.Length() - 1] = 0;
            
    return IPv4Packet::CalculateChecksum(fakePacket);

}
    
// Things to change
void TCPPacket::SetSourcePort(word16 port)
{
    m_sourcePort = NBO(port);
  
}

void TCPPacket::SetDestPort(word16 port)
{
    m_destPort = NBO(port);
  
}

void TCPPacket::SetTCPFlags(const TCPFlags& flags)
{

    m_flags[0] = flags.CWR;
    m_flags[1] = flags.ECN;
    m_flags[2] = flags.URG;
    m_flags[3] = flags.ACK;
    m_flags[4] = flags.PSH;
    m_flags[5] = flags.RST;
    m_flags[6] = flags.SYN;
    m_flags[7] = flags.FIN;
}

void TCPPacket::SetSequence(word32 seq)
{
    m_sequence = NBO(seq);
  
}

void TCPPacket::SetAckNum(word32 ack)
{
    m_ackNumber = NBO(ack);
  
}

void TCPPacket::SetWindowSize(word16 size)
{
    m_windowSize = NBO(size);
}

void TCPPacket::SetUrgentPointer(word16 ptr)
{
    m_urgentPointer = NBO(ptr);
  
}

void TCPPacket::SetPayload(ByteBuffer& payload)
{
    m_payload = payload;
}
    
// option manip someday
    
// Things you usally wouldn't change
void TCPPacket::BypassChecksum(bool bypass)
{
    m_bypassChecksum = bypass; 
}

void TCPPacket::BypassHeaderLength(bool bypass)
{
    m_bypassHdrLength = bypass;
}

