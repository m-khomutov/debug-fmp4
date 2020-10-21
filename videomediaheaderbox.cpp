#include "videomediaheaderbox.hh"

VideoMediaHeaderBox::VideoMediaHeaderBox( std::istream& is, uint32_t sz ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    is.read( (char*)&m_graphicsmode, sizeof(m_graphicsmode) );
    m_graphicsmode = be16toh( m_graphicsmode );
    for( int i(0); i < 3; ++i ) {
        is.read( (char*)&m_opcolor[i], sizeof(m_opcolor[i]) );
        m_opcolor[i] = be16toh( m_opcolor[i] );
    }
}

void VideoMediaHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " graphicsmode=" << m_graphicsmode <<  " opcolor={ ";
    for( size_t i(0); i < 3; ++i )
        out << int(m_opcolor[i]) << " ";
    out << "}";
}
