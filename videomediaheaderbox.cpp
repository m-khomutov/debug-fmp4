#include "videomediaheaderbox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const VideoMediaHeaderBox& vmhd ) {
    out << "version=" << int(vmhd.m_version) << " flags=" << std::hex << vmhd.m_flags << std::dec;
    out << " graphicsmode=" << vmhd.m_graphicsmode <<  " opcolor={ ";
    for( size_t i(0); i < 3; ++i )
        out << int(vmhd.m_opcolor[i]) << " ";
    out << "}";

    return out;
}

VideoMediaHeaderBox::VideoMediaHeaderBox( std::ifstream& f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    f.read( (char*)&m_graphicsmode, sizeof(m_graphicsmode) );
    m_graphicsmode = be16toh( m_graphicsmode );
    for( int i(0); i < 3; ++i ) {
        f.read( (char*)&m_opcolor[i], sizeof(m_opcolor[i]) );
        m_opcolor[i] = be16toh( m_opcolor[i] );
    }
}
