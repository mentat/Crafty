// --*-c++-*--
/*
    $Id: packet.h,v 1.1 2004/01/05 21:59:04 thementat Exp $
 
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
#ifndef PC_PACKET_H
#define PC_PACKET_H

#include <vector>
#include <stdexcept>
#include <memory>

#include "cryptopp/misc.h"

using namespace std;
using namespace CryptoPP;


// byte buffer, simple, no resizing!
class ByteBuffer
{
public:
    // On copy, new buffer takes control!
    ByteBuffer(const ByteBuffer& buf) 
    {   
        m_len = buf.m_len; 
        memcpy(m_buff, buf.m_buff, m_len);
    }
    
    ByteBuffer(byte* buff, word32 length) { m_buff = buff; m_len = length; }
    
    ByteBuffer() { m_buff = 0; m_len = 0; }
    
    explicit ByteBuffer(word32 length) { m_buff = new byte[length]; m_len = length; }
    
    ~ByteBuffer() { delete m_buff; }
    
    byte& operator[](word32 index) 
    { 
        if (index >= m_len) 
            throw out_of_range("ByteBuffer[]: Invalid index.");
        else
            return m_buff[index];
    }
    
    ByteBuffer& operator=(const ByteBuffer& buf)
    {
        if (m_buff == buf.m_buff)
            return *this;
            
        if (m_buff)
        {
            delete m_buff;
            m_len = 0;
        }
        
        m_len = buf.m_len; 
        m_buff = new byte[m_len];
        memcpy(m_buff, buf.m_buff, m_len);
        return *this;
    } 
    
    byte * Get() { return m_buff; }
    const byte * Get() const { return m_buff; }
    word32 Length() const { return m_len; }
    
    void CopyFrom(word32 startindex, word32 length, const byte* buffer)
    {
        if ((length == 0) || (buffer == 0))
            return;
        if ((startindex >= m_len) || (length + startindex > m_len))
            throw out_of_range("ByteBuffer::CopyTp: Invalid index.");
        else 
            memcpy(m_buff + startindex, buffer, length);
    } 
    
private:
    byte * m_buff;
    word32 m_len;
}; 

class Packet
{
    
public:
    Packet() {}
    
    virtual ~Packet() {}
    
    virtual ByteBuffer Craft() = 0;
    virtual unsigned int Length() = 0;
    
      
};

template <class T>
T NBO(const T& input)
{
#ifdef IS_BIG_ENDIAN
    return input;
#else
    return ByteReverse(input);
#endif
}

#endif //!PC_PACKET_H
