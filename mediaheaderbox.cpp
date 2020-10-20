#include "mediaheaderbox.hh"

MediaHeaderBox::MediaHeaderBox( std::istream & is, uint32_t sz ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    if( m_version == 1 ) {
        is.read( (char*)&m_creationTime, sizeof(m_creationTime) );
        m_creationTime = be64toh( m_creationTime );
        is.read( (char*)&m_modificationTime, sizeof(m_modificationTime) );
        m_modificationTime = be64toh( m_modificationTime );
        is.read( (char*)&m_timescale, sizeof(m_timescale) );
        m_timescale = be32toh( m_timescale );
        is.read( (char*)&m_duration, sizeof(m_duration) );
        m_duration = be64toh( m_duration );
    }
    else { // version==0
        is.read( (char*)&tmp, sizeof(tmp) );
        m_creationTime = be32toh( tmp );
        is.read( (char*)&tmp, sizeof(tmp) );
        m_modificationTime = be32toh( tmp );
        is.read( (char*)&tmp, sizeof(tmp) );
        m_timescale = be32toh( tmp );  // (video = fps rate ; audio = sample per sec. rate)
        is.read( (char*)&tmp, sizeof(tmp) );
        m_duration = be32toh( tmp );
    }
    /* 1/8 byte ISO language padding = 1-bit value set to 0
       7/8 bytes content language = 3 * 5-bits ISO 639-2 language code less 0x60
       example code for english = 0x15C7
    */
    uint16_t tmp16;
    is.read( (char*)&tmp16, sizeof(uint16_t) );
    tmp16 = be16toh( tmp16 );
    for( int i(0); i < 3; ++i )
        m_language[2 - i] = ((tmp16>>(i*5))&0x1f) + 0x60;

    is.read( (char*)&tmp16, sizeof(uint16_t) );  // pre_defined = 0;
}

void MediaHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "\nversion=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " ctime='"; Atom::ctime( out, m_creationTime );
    out << " mtime='"; Atom::ctime( out, m_modificationTime );
    out << " timescale=" << m_timescale << " duration=" << m_duration;
    out <<  " language=";
    for( size_t i(0); i < 3; ++i )
        out << std::string(1, m_language[i]);
}
