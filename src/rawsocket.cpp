// --*-c++-*--
/*
    $Id: rawsocket.cpp,v 1.1 2004/01/05 21:59:06 thementat Exp $
 
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

#include "rawsocket.h"

using namespace std;

RawSocket::RawSocket()
{

    m_libnet = libnet_init(
            LIBNET_LINK,                            /* injection type */
            NULL,                                   /* network interface */
            m_error);
            
    if (m_libnet == NULL)   
        cout << "Libnet" << endl;
  
}

RawSocket::~RawSocket()
{
    libnet_destroy(m_libnet);
    
}
  
   // send this packet, raw styles
bool RawSocket::Send(const ByteBuffer& buff)
{
  
    int ret = libnet_write_raw_ipv4(m_libnet, (u_int8_t *)buff.Get(),
    buff.Length());
    
    if (ret == -1)
        return false;
    return true;
  
}
    
bool RawSocket::IsOK()
{
    return false;
  
}
   
   // try to bind this port to listen
bool RawSocket::Listen(unsigned short port)
{
    return false;
  
}
  
   // see if there is data waiting on our bound port
bool RawSocket::IsData()
{
    return false;
  
}
 
   // see if we can count the amount of data waiting to be read
unsigned long RawSocket::CountData()
{
    return 0;
  
}
 
   // get length long piece of data, returns actual amount got
unsigned long RawSocket::Get(unsigned char * buffer, 
        const unsigned long length)
{
    return 0;
          
}
