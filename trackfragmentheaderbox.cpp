#include "trackfragmentheaderbox.hh"
#include <iostream>

std::ostream & operator <<( std::ostream& out, const TrackFragmentHeaderBox& tfhd ) {
    out << "version=" << int(tfhd.m_version) << "; flags=0x" << std::hex << tfhd.m_flags << std::dec << "; track id=" << tfhd.m_trackID;
    if( tfhd.m_flags & tfhd.kBaseDataOffsetPresent ) {
        out << "; base data offset=" <<tfhd.m_baseDataOffset;
    }
    else out << "; base data offset not present" ;

    if( tfhd.m_flags & tfhd.kSampleDescriptionIndexPresent ) {
        out << "; descrition index=" <<tfhd.m_sampleDescriptionIndex;
    }
    else out << "; description index not present";

    if( tfhd.m_flags & tfhd.kDefaultSampleDurationPresent ) {
        out << "; sample duration=" <<tfhd.m_defaultSampleDuration;
    }
    else out << "; sample duration not present" ;

    if( tfhd.m_flags & tfhd.kDefaultSampleSizePresent ) {
        out << "; sample size=" <<tfhd.m_defaultSampleSize;
    }
    else out << "; sample size not present" ;

    if( tfhd.m_flags & tfhd.kDefaultSampleFlagsPresent ) {
        out << "; sample flags={ ";
        tfhd.m_defaultSampleFlags->print( out );
	out << " }";
    }
    else out << "; sample flags not present" ;

    return out;
}

TrackFragmentHeaderBox::TrackFragmentHeaderBox( std::ifstream & f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    tmp = be32toh( tmp );

    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    f.read( (char*)&m_trackID, sizeof(m_trackID) );
    m_trackID = be32toh( m_trackID );

    if( m_flags & kBaseDataOffsetPresent ) {
        f.read( (char*)&m_baseDataOffset, sizeof(m_baseDataOffset) );
        m_baseDataOffset = be64toh( m_baseDataOffset );
    }
    if( m_flags & kSampleDescriptionIndexPresent ) {
        f.read( (char*)&m_sampleDescriptionIndex, sizeof(m_sampleDescriptionIndex) );
        m_sampleDescriptionIndex = be32toh( m_sampleDescriptionIndex );
    }
    if( m_flags & kDefaultSampleDurationPresent ) {
        f.read( (char*)&m_defaultSampleDuration, sizeof(m_defaultSampleDuration) );
        m_defaultSampleDuration = be32toh( m_defaultSampleDuration );
    }
    if( m_flags & kDefaultSampleSizePresent ) {
        f.read( (char*)&m_defaultSampleSize, sizeof(m_defaultSampleSize) );
        m_defaultSampleSize = be32toh( m_defaultSampleSize );
    }
    if( m_flags & kDefaultSampleFlagsPresent ) {
        m_defaultSampleFlags.reset( new TrackExtendsBox::SampleFlags( f ) );
    }
}
