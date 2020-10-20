#include "trackfragmentheaderbox.hh"

TrackFragmentHeaderBox::TrackFragmentHeaderBox( std::istream & is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    tmp = be32toh( tmp );

    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    is.read( (char*)&m_trackID, sizeof(m_trackID) );
    m_trackID = be32toh( m_trackID );

    if( m_flags & kBaseDataOffsetPresent ) {
        is.read( (char*)&m_baseDataOffset, sizeof(m_baseDataOffset) );
        m_baseDataOffset = be64toh( m_baseDataOffset );
    }
    if( m_flags & kSampleDescriptionIndexPresent ) {
        is.read( (char*)&m_sampleDescriptionIndex, sizeof(m_sampleDescriptionIndex) );
        m_sampleDescriptionIndex = be32toh( m_sampleDescriptionIndex );
    }
    if( m_flags & kDefaultSampleDurationPresent ) {
        is.read( (char*)&m_defaultSampleDuration, sizeof(m_defaultSampleDuration) );
        m_defaultSampleDuration = be32toh( m_defaultSampleDuration );
    }
    if( m_flags & kDefaultSampleSizePresent ) {
        is.read( (char*)&m_defaultSampleSize, sizeof(m_defaultSampleSize) );
        m_defaultSampleSize = be32toh( m_defaultSampleSize );
    }
    if( m_flags & kDefaultSampleFlagsPresent ) {
        m_defaultSampleFlags.reset( new TrackExtendsBox::SampleFlags( is ) );
    }
}

void TrackFragmentHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "\nversion=" << int(m_version) << "; flags=0x" << std::hex << m_flags << std::dec << "; track id=" << m_trackID;
    if( m_flags & kBaseDataOffsetPresent ) {
        out << "; base data offset=" << m_baseDataOffset;
    }
    else out << "; base data offset not present" ;

    if( m_flags & kSampleDescriptionIndexPresent ) {
        out << "; descrition index=" << m_sampleDescriptionIndex;
    }
    else out << "; description index not present";

    if( m_flags & kDefaultSampleDurationPresent ) {
        out << "; sample duration=" << m_defaultSampleDuration;
    }
    else out << "; sample duration not present" ;

    if( m_flags & kDefaultSampleSizePresent ) {
        out << "; sample size=" << m_defaultSampleSize;
    }
    else out << "; sample size not present" ;

    if( m_flags & kDefaultSampleFlagsPresent ) {
        out << "; sample flags={ ";
        m_defaultSampleFlags->fout( out );
        out << " }";
    }
    else out << "; sample flags not present" ;
}