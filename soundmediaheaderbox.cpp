#include "soundmediaheaderbox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const SoundMediaHeaderBox& smhd ) {
    out << "version=" << int(smhd.m_version) << " flags=" << std::hex << smhd.m_flags << std::dec;
    out << " balance=" << smhd.m_balance;
    return out;
}

SoundMediaHeaderBox::SoundMediaHeaderBox( std::ifstream& f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    f.read( (char*)&m_balance, sizeof(m_balance) );
    m_balance = be16toh( m_balance );
    f.read( (char*)&tmp, sizeof(uint16_t) );  // reserved = 0;
}
