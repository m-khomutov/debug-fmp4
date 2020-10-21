#include "movieheaderbox.hh"

MovieHeaderBox::MovieHeaderBox( std::istream & is, uint32_t sz ) : Atom( is ) {
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
        m_timescale = be32toh( tmp );
        is.read( (char*)&tmp, sizeof(tmp) );
        m_duration = be32toh( tmp );
    }
    is.read( (char*)&m_rate, sizeof(m_rate) );
    m_rate = be32toh( m_rate );
    is.read( (char*)&m_volume, sizeof(m_volume) );
    m_volume = be16toh( m_volume );

    is.read( (char*)&tmp, sizeof(uint16_t) ); //const bit(16) reserved = 0;
    for( int i(0); i < 2; ++i )
        is.read( (char*)&tmp, sizeof(tmp) );  // const unsigned int(32)[2] reserved = 0;

    for( int i(0); i < 9; ++i ) {
        is.read( (char*)&tmp, sizeof(tmp) );
        m_matrix[i] = be32toh( tmp );
    }

    for( int i(0); i < 6; ++i )
        is.read( (char*)&tmp, sizeof(tmp) );  // bit(32)[6] pre_defined = 0;

    is.read( (char*)&tmp, sizeof(uint32_t) );
    m_nextTrackID = be32toh( tmp );
}

void MovieHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " ctime='"; Atom::ctime( out, m_creationTime );
    out << " mtime='"; Atom::ctime( out, m_modificationTime );
    out << " timescale=" << m_timescale << " duration=" << m_duration;
    out << std::hex << " rate=" << m_rate << " volume=" << m_volume << " matrix={";
    for( size_t i(0); i < 9; ++i ) {
        out << m_matrix[i];
        out << (i != 8 ? "," : "}");
    }
    out << std::dec << " next track id=" << m_nextTrackID;
}
