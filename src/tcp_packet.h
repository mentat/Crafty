// --*-c++-*--
/*
    $Id: tcp_packet.h,v 1.1 2004/01/05 21:59:05 thementat Exp $
 
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
#ifndef TCP_PACKET_H
#define TCP_PACKET_H

#include <vector>
#include "ipv4_packet.h"

using namespace std;
using namespace CryptoPP;

#define BASE_TCP_HEADER_LENGTH 20

class TCPPacket: public IPv4Packet
{
public:
    TCPPacket();
    virtual ~TCPPacket() {}
   
    class TCPFlags {
       public:
       TCPFlags() { CWR = ECN = URG = ACK = PSH = RST = SYN = FIN = false; }
        bool CWR; // Congestion window reduced
        bool ECN; // Echo
        bool URG;
        bool ACK;
        bool PSH;
        bool RST;
        bool SYN;
        bool FIN;
    };
 
    virtual unsigned int Length();
    
    virtual ByteBuffer Craft();
    
    // Things to change
    void SetSourcePort(word16 port);
    void SetDestPort(word16 port);
    void SetTCPFlags(const TCPFlags& flags);
    void SetSequence(word32 seq);
    void SetAckNum(word32 ack);
    void SetWindowSize(word16 size);
    void SetUrgentPointer(word16 ptr);
    void SetPayload(ByteBuffer& payload);
    void SetChecksum(word16 check);
    void SetHeaderLength(bitset<4>& length);
    
    ByteBuffer GetPayload();
    word16 GetTCPChecksum() const;
    
    // option manip someday
    
    // Things you usally wouldn't change
    void BypassChecksum(bool bypass);
    void BypassHeaderLength(bool bypass);

protected:

    virtual ByteBuffer DoCraft(ByteBuffer& buff);
    virtual word16 CalculateChecksum(ByteBuffer& tcp, word16 overide = 0);
    word16 TCPLength();


private:
 
    // true if we want to bypass these auto-determined things
    bool m_bypassChecksum;
    bool m_bypassHdrLength;
 
    // TCP header
    word16 m_sourcePort; // 16 bits
    word16 m_destPort; // 16 bits
    word32 m_sequence; // 32 bits
    word32 m_ackNumber; // 32 bits
    bitset<4> m_headerLength; // 4 bits
    bitset<6> m_reserved; // 6 bits
    bitset<6> m_flags; // 6 bits, see tcpflags struct
    word16 m_windowSize;
    word16 m_tcpChecksum;
    word16 m_urgentPointer;
    
    // and options, not implemented yet
    ByteBuffer m_options;
   
    // payload
    ByteBuffer m_payload;
};

#endif
