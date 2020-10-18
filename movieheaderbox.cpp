#include "movieheaderbox.hh"

void printMp4Time( std::ostream & out, time_t t ) {
    struct tm * tp = localtime( &t );
    tp->tm_year += 1904;

    out << tp->tm_mday << "/" << tp->tm_mon+1 << "/" << tp->tm_year << " "
        << tp->tm_hour << ":" << tp->tm_min << ":" << tp->tm_sec;
}

std::ostream & operator <<( std::ostream& out, const MovieHeaderBox& mvhd ) {
    out << "version=" << int(mvhd.m_version) << " flags=" << std::hex << mvhd.m_flags << std::dec;
    out << " ctime='"; printMp4Time( out, mvhd.m_creationTime );
    out << " mtime='"; printMp4Time( out, mvhd.m_modificationTime );
    out << " timescale=" << mvhd.m_timescale << " duration=" << mvhd.m_duration;
    out << std::hex << " rate=" << mvhd.m_rate << " volume=" << mvhd.m_volume << " matrix={";
    for( size_t i(0); i < 9; ++i ) {
        out << mvhd.m_matrix[i];
        out << (i != 8 ? "," : "}");
    }
    out << std::dec << " next track id=" << mvhd.m_nextTrackID;

    return out;
}

MovieHeaderBox::MovieHeaderBox( std::ifstream & f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );

    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    if( m_version == 1 ) {
        f.read( (char*)&m_creationTime, sizeof(m_creationTime) );
        m_creationTime = be64toh( m_creationTime );
        f.read( (char*)&m_modificationTime, sizeof(m_modificationTime) );
        m_modificationTime = be64toh( m_modificationTime );
        f.read( (char*)&m_timescale, sizeof(m_timescale) );
        m_timescale = be32toh( m_timescale );
        f.read( (char*)&m_duration, sizeof(m_duration) );
        m_duration = be64toh( m_duration );
    }
    else { // version==0
        f.read( (char*)&tmp, sizeof(tmp) );
        m_creationTime = be32toh( tmp );
        f.read( (char*)&tmp, sizeof(tmp) );
        m_modificationTime = be32toh( tmp );
        f.read( (char*)&tmp, sizeof(tmp) );
        m_timescale = be32toh( tmp );
        f.read( (char*)&tmp, sizeof(tmp) );
        m_duration = be32toh( tmp );
    }
    f.read( (char*)&m_rate, sizeof(m_rate) );
    m_rate = be32toh( m_rate );
    f.read( (char*)&m_volume, sizeof(m_volume) );
    m_volume = be16toh( m_volume );

    f.read( (char*)&tmp, sizeof(uint16_t) ); //const bit(16) reserved = 0;
    for( int i(0); i < 2; ++i )
        f.read( (char*)&tmp, sizeof(tmp) );  // const unsigned int(32)[2] reserved = 0;

    for( int i(0); i < 9; ++i ) {
        f.read( (char*)&tmp, sizeof(tmp) );
        m_matrix[i] = be32toh( tmp );
    }

    for( int i(0); i < 6; ++i )
        f.read( (char*)&tmp, sizeof(tmp) );  // bit(32)[6] pre_defined = 0;

    f.read( (char*)&tmp, sizeof(uint32_t) );
    m_nextTrackID = be32toh( tmp );
}
