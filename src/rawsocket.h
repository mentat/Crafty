// --*-c++-*--
/*
    $Id: rawsocket.h,v 1.1 2004/01/05 21:59:06 thementat Exp $
 
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
#ifndef RAWSOCKET_H
#define RAWSOCKET_H

// A platform independent raw socket
#include "packet.h"
#include "libnet.h"

class RawSocket
{
public:
	
	RawSocket();
	virtual ~RawSocket();
	
	// send this packet, raw styles
	virtual bool Send(const ByteBuffer& buff);
    
    virtual bool IsOK();
	
	// try to bind this port to listen
	virtual bool Listen(unsigned short port);
	
	// see if there is data waiting on our bound port
	virtual bool IsData();
	
	// see if we can count the amount of data waiting to be read
	virtual unsigned long CountData();
	
	// get length long piece of data, returns actual amount got
	virtual unsigned long Get(unsigned char * buffer, 
        const unsigned long length);
        

private:
    
    libnet_t * m_libnet;
    int8_t m_error[LIBNET_ERRBUF_SIZE];
	
	
};

#endif
