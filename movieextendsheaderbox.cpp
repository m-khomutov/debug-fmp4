#include "movieextendsheaderbox.hh"

std::ostream & operator <<( std::ostream& out, const MovieExtendsHeaderBox& mehd ) {
    out << "version=" << int(mehd.m_version) << " flags=" << std::hex << mehd.m_flags << std::dec << " fragment duration=" <<mehd.m_fragmentDuration;
    return out;
}

MovieExtendsHeaderBox::MovieExtendsHeaderBox( std::ifstream & f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    if( m_version == 1 ) {
        f.read( (char*)&m_fragmentDuration, sizeof(m_fragmentDuration) );
        m_fragmentDuration = be64toh( m_fragmentDuration );
    }
    else { // version==0
        f.read( (char*)&tmp, sizeof(tmp) );
        m_fragmentDuration = be32toh( tmp );
    }
}
