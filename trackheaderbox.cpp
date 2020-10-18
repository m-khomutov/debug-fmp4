#include "trackheaderbox.hh"

extern void printMp4Time( std::ostream & out, time_t t );

std::ostream & operator <<( std::ostream& out, const TrackHeaderBox& tkhd ) {
    out << "version=" << int(tkhd.m_version) << " flags=" << std::hex << tkhd.m_flags << std::dec;
    out << " ctime='"; printMp4Time( out, tkhd.m_creationTime );
    out << " mtime='"; printMp4Time( out, tkhd.m_modificationTime );
    out << " track id=" << tkhd.m_trackID << " duration=" << tkhd.m_duration;
    out << std::hex << " layer=" << tkhd.m_layer << " alternate group=" << tkhd.m_alternateGroup << " volume: " << tkhd.m_volume << " matrix={";
    for( size_t i(0); i < 9; ++i ) {
        out << tkhd.m_matrix[i];
        out << (i != 8 ? "," : "}");
    }
    out << std::dec << " width=" << tkhd.m_width << " height=" << tkhd.m_height;

    return out;
}

TrackHeaderBox::TrackHeaderBox( std::ifstream & f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    if( m_version == 1 ) {
        f.read( (char*)&m_creationTime, sizeof(m_creationTime) );
        m_creationTime = be64toh( m_creationTime );
        f.read( (char*)&m_modificationTime, sizeof(m_modificationTime) );
        m_modificationTime = be64toh( m_modificationTime );
        f.read( (char*)&m_trackID, sizeof(m_trackID) );
        m_trackID = be32toh( m_trackID );
        f.read( (char*)&tmp, sizeof(tmp) );  // reserved = 0
        f.read( (char*)&m_duration, sizeof(m_duration) );
        m_duration = be64toh( m_duration );
    }
    else { // version==0
        f.read( (char*)&tmp, sizeof(tmp) );
        m_creationTime = be32toh( tmp );
        f.read( (char*)&tmp, sizeof(tmp) );
        m_modificationTime = be32toh( tmp );
        f.read( (char*)&tmp, sizeof(tmp) );
        m_trackID = be32toh( tmp );
        f.read( (char*)&tmp, sizeof(tmp) );  // reserved = 0
        f.read( (char*)&tmp, sizeof(tmp) );
        m_duration = be32toh( tmp );
    }
    for( int i(0); i < 2; ++i )
        f.read( (char*)&tmp, sizeof(tmp) );  // const unsigned int(32)[2] reserved = 0;

    f.read( (char*)&m_layer, sizeof(m_layer) );
    m_layer = be16toh( m_layer );
    f.read( (char*)&m_alternateGroup, sizeof(m_alternateGroup) );
    m_alternateGroup = be16toh( m_alternateGroup );
    f.read( (char*)&m_volume, sizeof(m_volume) );
    m_volume = be16toh( m_volume );

    f.read( (char*)&tmp, sizeof(uint16_t) ); //const bit(16) reserved = 0;
    for( int i(0); i < 9; ++i ) {
        f.read( (char*)&tmp, sizeof(tmp) );
        m_matrix[i] = be32toh( tmp );
    }
    f.read( (char*)&tmp, sizeof(uint32_t) );
    m_width = be32toh( tmp );
    f.read( (char*)&tmp, sizeof(uint32_t) );
    m_height = be32toh( tmp );
}
