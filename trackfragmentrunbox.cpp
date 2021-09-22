#include "trackfragmentrunbox.hh"

TrackFragmentRunBox::Sample::Sample( std::istream &is, uint32_t flags ) : m_trflags( flags ) {
    if( flags & TrackFragmentRunBox::kSampleDurationPresent ) {
        is.read( (char*)&m_sampleDuration, sizeof(m_sampleDuration) );
        m_sampleDuration = be32toh( m_sampleDuration );
    }
    if( flags & TrackFragmentRunBox::kSampleSizePresent ) {
        is.read( (char*)&m_sampleSize, sizeof(m_sampleSize) );
        m_sampleSize = be32toh( m_sampleSize );
    }
    if( flags & TrackFragmentRunBox::kSampleFlagsPresent ) {
        is.read( (char*)&m_sampleFlags, sizeof(m_sampleFlags) );
        m_sampleFlags = be32toh( m_sampleFlags );
    }
    if( flags & TrackFragmentRunBox::kSampleCompositionTimeOffsetsPresent ) {
        is.read( (char*)&m_sampleCompositionTimeOffset, sizeof(m_sampleCompositionTimeOffset) );
        m_sampleCompositionTimeOffset = be32toh( m_sampleCompositionTimeOffset );
    }
}

void TrackFragmentRunBox::Sample::fout( std::ostream &out, const Atom * atom ) {
    out << "{";
    if( m_trflags & TrackFragmentRunBox::kSampleDurationPresent ) {
        out << m_sampleDuration << ",";
    }
    else
        out << ",";
    if( m_trflags & TrackFragmentRunBox::kSampleSizePresent ) {
        out << m_sampleSize << ",";
    }
    else
        out << ",";
    if( m_trflags & TrackFragmentRunBox::kSampleFlagsPresent ) {
        out << std::hex << m_sampleFlags << std::dec << ",";
    }
    else
        out << ",";
    if( m_trflags & TrackFragmentRunBox::kSampleCompositionTimeOffsetsPresent ) {
        out << m_sampleCompositionTimeOffset;
    }
    out << "}";
}

TrackFragmentRunBox::TrackFragmentRunBox( std::istream& is ) : Atom( is ){
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    tmp = be32toh( tmp );

    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    is.read( (char*)&m_sampleCount, sizeof(m_sampleCount) );
    m_sampleCount = be32toh( m_sampleCount );

    if( m_flags & kDataOffsetPresent ) {
        is.read( (char*)&m_dataOffset, sizeof(m_dataOffset) );
        m_dataOffset = be32toh( m_dataOffset );
    }
    if( m_flags & kFirstSampleFlagsPresent ) {
        is.read( (char*)&m_firstSampleFlags, sizeof(m_firstSampleFlags) );
        m_firstSampleFlags = be32toh( m_firstSampleFlags );
    }
    for( uint32_t i(0); i < m_sampleCount; ++i )
        m_samples.push_back( Sample( is, m_flags ) );
}

void TrackFragmentRunBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " trFlags=" << std::hex << m_flags << std::dec;
    if( m_flags & TrackFragmentRunBox::kDataOffsetPresent )
        out << " data offset: " << m_dataOffset;
    if( m_flags & TrackFragmentRunBox::kFirstSampleFlagsPresent )
        out << " first sample flags: " << std::hex << m_firstSampleFlags << std::dec;

    out << " samples " << m_sampleCount << " {duration,size,flags,timeoffset}: ";
    for( auto sample: m_samples )
        sample.fout( out, this );
}
