// --*-c++-*--
/*
    $Id: main.cpp,v 1.1 2004/01/05 21:59:05 thementat Exp $
 
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
#include <fstream>

#include "packet.h"
#include "ipv4_packet.h"
#include "tcp_packet.h"

using namespace std;

int main()
{
	//IPv4Packet packet;
    
    TCPPacket tcp;
    tcp.SetSourcePort(1171);
    tcp.SetDestPort(25);
    tcp.SetFlags(true, false);
    tcp.SetID(3314);
    tcp.SetSourceAddress(0x9807e98f);
    tcp.SetDestAddress(0x980101ab);
    tcp.SetTTL(128);
    tcp.SetSequence(0x9c0bdc94);
    tcp.SetAckNum(0x89569e1f);
    tcp.SetWindowSize(17197);
    
    TCPPacket::TCPFlags flags;
    flags.ACK = true;
    flags.PSH = true;
    
    ByteBuffer payload(6);
    payload[0] = 0x52;
    payload[1] = 0x53;
    payload[2] = 0x45;
    payload[3] = 0x54;
    payload[4] = 0x0d;
    payload[5] = 0x0a;
    tcp.SetPayload(payload);
    
    tcp.SetTCPFlags(flags);
    
    
    
    ByteBuffer packet = tcp.Craft();
    
    ofstream out("packet.bin", ios_base::binary);
    
    for( unsigned int i = 0; i < packet.Length(); i++)
        out << packet[i];
    
    
	return 0;
}
