// --*-c++-*--
/*
    $Id: udp_packet.h,v 1.1 2004/01/05 21:59:05 thementat Exp $
 
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
#ifndef UDP_PACKET_H
#define UDP_PACKET_H

#include "ipv4_packet.h"

class UDPPacket: public IPv4Packet
{
   public:
    
   UDPPacket() {}
 
};

#endif
