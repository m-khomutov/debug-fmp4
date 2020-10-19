#include "movieextendsheaderbox.hh"

MovieExtendsHeaderBox::MovieExtendsHeaderBox( std::istream & is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    if( m_version == 1 ) {
        is.read( (char*)&m_fragmentDuration, sizeof(m_fragmentDuration) );
        m_fragmentDuration = be64toh( m_fragmentDuration );
    }
    else { // version==0
        is.read( (char*)&tmp, sizeof(tmp) );
        m_fragmentDuration = be32toh( tmp );
    }
}

void MovieExtendsHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec << " fragment duration=" <<m_fragmentDuration;
}