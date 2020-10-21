#include "trackheaderbox.hh"

TrackHeaderBox::TrackHeaderBox( std::istream & is, uint32_t sz ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    if( m_version == 1 ) {
        is.read( (char*)&m_creationTime, sizeof(m_creationTime) );
        m_creationTime = be64toh( m_creationTime );
        is.read( (char*)&m_modificationTime, sizeof(m_modificationTime) );
        m_modificationTime = be64toh( m_modificationTime );
        is.read( (char*)&m_trackID, sizeof(m_trackID) );
        m_trackID = be32toh( m_trackID );
        is.read( (char*)&tmp, sizeof(tmp) );  // reserved = 0
        is.read( (char*)&m_duration, sizeof(m_duration) );
        m_duration = be64toh( m_duration );
    }
    else { // version==0
        is.read( (char*)&tmp, sizeof(tmp) );
        m_creationTime = be32toh( tmp );
        is.read( (char*)&tmp, sizeof(tmp) );
        m_modificationTime = be32toh( tmp );
        is.read( (char*)&tmp, sizeof(tmp) );
        m_trackID = be32toh( tmp );
        is.read( (char*)&tmp, sizeof(tmp) );  // reserved = 0
        is.read( (char*)&tmp, sizeof(tmp) );
        m_duration = be32toh( tmp );
    }
    for( int i(0); i < 2; ++i )
        is.read( (char*)&tmp, sizeof(tmp) );  // const unsigned int(32)[2] reserved = 0;

    is.read( (char*)&m_layer, sizeof(m_layer) );
    m_layer = be16toh( m_layer );
    is.read( (char*)&m_alternateGroup, sizeof(m_alternateGroup) );
    m_alternateGroup = be16toh( m_alternateGroup );
    is.read( (char*)&m_volume, sizeof(m_volume) );
    m_volume = be16toh( m_volume );

    is.read( (char*)&tmp, sizeof(uint16_t) ); //const bit(16) reserved = 0;
    for( int i(0); i < 9; ++i ) {
        is.read( (char*)&tmp, sizeof(tmp) );
        m_matrix[i] = be32toh( tmp );
    }
    is.read( (char*)&tmp, sizeof(uint32_t) );
    m_width = be32toh( tmp );
    is.read( (char*)&tmp, sizeof(uint32_t) );
    m_height = be32toh( tmp );
}

void TrackHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " ctime='"; Atom::ctime( out, m_creationTime );
    out << " mtime='"; Atom::ctime( out, m_modificationTime );
    out << " track id=" << m_trackID << " duration=" << m_duration;
    out << std::hex << " layer=" << m_layer << " alternate group=" << m_alternateGroup << " volume: " << m_volume << " matrix={";
    for( size_t i(0); i < 9; ++i ) {
        out << m_matrix[i];
        out << (i != 8 ? "," : "}");
    }
    out << std::dec << " width=" << m_width << " height=" << m_height;
}
