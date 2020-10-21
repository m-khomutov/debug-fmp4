#include "soundmediaheaderbox.hh"

SoundMediaHeaderBox::SoundMediaHeaderBox( std::istream& is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    is.read( (char*)&m_balance, sizeof(m_balance) );
    m_balance = be16toh( m_balance );
    is.read( (char*)&tmp, sizeof(uint16_t) );  // reserved = 0;
}

void SoundMediaHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec << " balance=" << m_balance;
}
