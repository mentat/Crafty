// --*-c++-*--
/*
    $Id: ipv4_packet.cpp,v 1.1 2004/01/05 21:59:04 thementat Exp $
 
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
#include "ipv4_packet.h"
#include <iostream>

using namespace std;

IPv4Packet::IPv4Packet()
{
    m_bypassLength = false;
    m_bypassHdrLength = false;
    m_bypassIPChecksum = false;
    m_version = bitset<4>(0x4); 

    m_length = 5; // smallest length of ip header only
    m_id = 0; // identification
    m_fragOffset = 0; // 13 fragment offset in 64 bit units
    m_ttl = 64; // default
 
    m_checksum = 0; 
    m_sourceAddress = 0;
    m_destAddress = 0;
 
}

void IPv4Packet::SetDestAddress(const word32& ip_addr)
{
    m_destAddress = NBO(ip_addr);
}

void IPv4Packet::SetSourceAddress(const word32& ip_addr)
{
    m_sourceAddress = NBO(ip_addr);

}

void IPv4Packet::SetID(const word16& id)
{
    m_id = NBO(id);
}

void IPv4Packet::SetMF(bool may_frag)
{
  if (may_frag)
        m_flags.set(1);
  else
        m_flags.reset(1);
}

void IPv4Packet::SetLF(bool last_frag)
{
  if (last_frag)
        m_flags.set(2);
  else
        m_flags.reset(2);
}

void IPv4Packet::SetTOS(TOS tos)
{
  // not yet
}

void IPv4Packet::SetTTL(byte ttl)
{
    m_ttl = ttl;
}

void IPv4Packet::SetProtocol(byte proto)
{
    m_protocol = proto;
}

void IPv4Packet::SetFragOffset(word16 off)
{
    m_fragOffset = NBO(off);
}

word16 IPv4Packet::CalculateChecksum(ByteBuffer& buff, word16 override)
{
  
    register word16 * buff16 = (word16 *)buff.Get();
    register word32 length16 = buff.Length();
    
    if (override != 0)
        length16 = override;

    register word32 sum = 0;    
    word16 answer = 0;    
    //register word16 *w = addr;    
    //register word32 nleft = len;
    
    /*    
    * * Our algorithm is simple, using a 32 bit accumulator (sum), we add    
    * * sequential 16 bit words to it, and at the end, fold back all the    
    * * carry bits from the top 16 bits into the lower 16 bits.    
    * */
    
    while (length16 > 1) {    
        sum += *buff16++;
        length16 -= 2;
    }

    /* mop up an odd byte, if necessary */
    
    if (length16 == 1) {
       word16 odd;
        if (override != 0)
            odd = ((buff[override - 1] << 8) & 0xff00);
        else
            odd = ((buff[buff.Length() - 1] << 8) & 0xff00);
        
        //odd |= (word16)((
        //*(byte *)(&answer) = *(byte *)buff16 ;
        sum += odd; 
    }

    /* add back carry outs from top 16 bits to low 16 bits */
    
    sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */    
    sum += (sum >> 16); /* add carry */
    
    answer = ~sum; /* truncate to 16 bits */
    
    return(answer);
  
}
/*
unsigned short csum (unsigned short *buf, int nwords)
{
unsigned long sum;
for (sum = 0; nwords > 0; nwords--)
sum += *buf++;
sum = (sum >> 16) + (sum & 0xffff);
sum += (sum >> 16);
return ~sum;
}*/
	
/// return crafted packet of length, length
ByteBuffer IPv4Packet::Craft() 
{
    ByteBuffer dummy;
    return DoCraft(dummy);
}

ByteBuffer IPv4Packet::DoCraft(ByteBuffer& payload)
{ 
    ByteBuffer ipv4(BASE_IPV4_HEADER_LENGTH + m_options.Length() + payload.Length());
   
    ipv4[0] = (byte) m_version.to_ulong() << 4;
    
    if (!m_bypassHdrLength)
        m_hdrLength = bitset<4>((BASE_IPV4_HEADER_LENGTH + m_options.Length())/4);
    
    ipv4[0] |=(byte) m_hdrLength.to_ulong();
    
    ipv4[1] = (byte) m_typeOfService.to_ulong();
    
    if (!m_bypassLength)
        m_length = NBO((word16)(BASE_IPV4_HEADER_LENGTH + m_options.Length() + payload.Length()));
    
    ipv4[2] = (byte) (m_length & 0xff);
    ipv4[3] = (byte) ((m_length >> 8) & 0xff);
    
    ipv4[4] = (byte) (m_id & 0xff);
    ipv4[5] = (byte) ((m_id >> 8) & 0xff);
    
    ipv4[6] = (byte) (m_flags.to_ulong() << 5) ;
    ipv4[6] |= (byte) (m_fragOffset >> 8);
    
    ipv4[7] = (byte) m_fragOffset;
    
    ipv4[8] = m_ttl;
    ipv4[9] = m_protocol;
    
    if (!m_bypassIPChecksum)
        m_checksum = 0;
        
    ipv4[10] = (m_checksum & 0xff);
    ipv4[11] = ((m_checksum >> 8) & 0xff);
    
    ipv4[12] = (byte) (m_sourceAddress & 0xff);
    ipv4[13] = (byte) ((m_sourceAddress >> 8) & 0xff);
    ipv4[14] = (byte) ((m_sourceAddress >> 16) & 0xff);
    ipv4[15] = (byte) ((m_sourceAddress >> 24) & 0xff);
    
    ipv4[16] = (byte) (m_destAddress & 0xff);
    ipv4[17] = (byte) ((m_destAddress >> 8) & 0xff);
    ipv4[18] = (byte) ((m_destAddress >> 16) & 0xff);
    ipv4[19] = (byte) ((m_destAddress >> 24) & 0xff);
    
    /* options */  
    
    ipv4.CopyFrom(20, m_options.Length(), m_options.Get());
    
    // Checksum
    
    if (!m_bypassIPChecksum)
    {
        m_checksum = CalculateChecksum(ipv4, 20 + m_options.Length());
        ipv4[10] = (m_checksum & 0xff);
        ipv4[11] = ((m_checksum >> 8) & 0xff);
    }  
    
    /* payload */
    
    ipv4.CopyFrom(20 + m_options.Length(), payload.Length(), payload.Get());
    
    cout << "Craft done\n";
    return ipv4;
}
