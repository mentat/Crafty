#ifndef IPV4_PACKET_H
#define IPV4_PACKET_H

#include <bitset>
#include <vector>
#include "cryptopp/misc.h"
#include "packet.h"

using namespace std;
using namespace CryptoPP;

#define BASE_IPV4_HEADER_LENGTH 20

class IPv4Packet: public Packet
{
public:
    
    IPv4Packet();
    virtual ~IPv4Packet() {}
    
    enum TOS {
        Default = 0x0,
        MinCost = 0x1,
        MaxReliability = 0x2,
        MaxThroughput = 0x4,
        MinDelay = 0x8,
        MaxSecurity = 0xf
    };
 
    enum Protocol {
        ICMP = 1,
        IGMP = 2,
        IPinIP = 4,
        TCP = 6,
        UDP = 17
    };
 
    /// return the entire length of the packet to be forged so far
    /// equals header length plus payload length
    virtual unsigned int Length() {
        return IPLength();
    }
    
    inline unsigned int IPLength() {
      return BASE_IPV4_HEADER_LENGTH + m_options.Length();
    }
    
    /// Length of lower level payload
    virtual word16 PayloadLength() { return 0; }
  
    void SetDestAddress(const word32& ip_addr);
    void SetSourceAddress(const word32& ip_addr);
    void SetID(const word16& id);
    void SetMF(bool may_frag);
    void SetLF(bool last_frag);
    void SetTOS(TOS tos);
    void SetTTL(byte ttl);
    void SetProtocol(byte proto);
    void SetFragOffset(word16 off);
    
    word32 GetSourceAddress() const { return m_sourceAddress; }
    word32 GetDestAddress() const { return m_destAddress; }
    
    byte GetProtocol() const { return m_protocol; }
  
    /// return crafted packet of length, length
    virtual ByteBuffer Craft();
   
protected:
 
    virtual ByteBuffer DoCraft(ByteBuffer& payload);
    virtual word16 CalculateChecksum(ByteBuffer& buff, word16 overide = 0);
          
private:

    bool m_bypassLength;
    bool m_bypassHdrLength;
    bool m_bypassIPChecksum;
   
    // IP header
    bitset<4> m_version; // 4 bits ipv4 is 0x4, v6 is 0v6
    bitset<4> m_hdrLength; // 4 bits in 32 bit words, minimum 0x4, max 0xF
    bitset<8> m_typeOfService;
    word16 m_length; // total length in octets, maximum, 65,535 bytes
    word16 m_id; // identification
    bitset<3> m_flags; // 3 bits, one unused
    word16 m_fragOffset; // 13 fragment offset in 64 bit units
    byte m_ttl; 
    byte m_protocol;
    word16 m_checksum; // 16 bit ones complement addition of all 16 bit words in header, initialy zero
    word32 m_sourceAddress;
    word32 m_destAddress;
 
    /*  options not available in this release */
    ByteBuffer m_options;
   
};

#endif
