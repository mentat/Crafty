// --*-c++-*--
/*
    $Id: linux_raw_socket.cpp,v 1.1 2004/01/05 21:59:06 thementat Exp $
 
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
#if 0
#include <sys/socket.h>
#include <features.h>    /* for the glibc version number */
#if __GLIBC__ >= 2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>     /* the L2 protocols */
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>   /* The L2 protocols */
#endif


#include "linux_raw_socket.h"
#include "packet.h"

LinuxRawSocket::LinuxRawSocket() 
{
    // use SOCK_RAW for hard core level 2
    m_socket = socket(PF_PACKET, SOCK_DGRAM, 0);
                                                                                    
                                                                                    
    perror("LinuxRawSocket()");

}
LinuxRawSocket::~LinuxRawSocket() 
{
    //if (m_socket > 0)
    //    close(m_socket);
}
    
bool LinuxRawSocket::IsOK()
{
  
    if (m_socket == -1)
        return false;
    return true;  
}
    
    // send this packet, raw styles
bool LinuxRawSocket::Send(const ByteBuffer& buff)
{
    sockaddr sa;
    sa.sa_family = AF_INET;

    if (sendto(m_socket, buff.Get(), buff.Length(), 0, &sa, sizeof(sa)) < 0)
    {
                                                                                        
        perror("Send");
        return false;
    }
    return true;
}
    
    // try to bind this port to listen
bool LinuxRawSocket::Listen(unsigned short port)
{
    return false;
}
    
    // see if there is data waiting on our bound port
bool LinuxRawSocket::IsData()
{
    return false;
}
   
    // see if we can count the amount of data waiting to be read
unsigned long LinuxRawSocket::CountData()
{
    return 0;
}
   
    // get length long piece of data, returns actual amount got
unsigned long LinuxRawSocket::Get(unsigned char * buffer, 
    const unsigned long length)
{
    return 0;
}
#endif

