#include "trackfragmentrunbox.hh"

#include <iostream>
std::ostream & operator <<( std::ostream& out, const TrackFragmentRunBox& trun ) {
    out << "version=" << int(trun.m_version) << " trFlags=" << std::hex << trun.m_flags << std::dec;
    if( trun.m_flags & TrackFragmentRunBox::kDataOffsetPresent )
        out << " data offset: " << trun.m_dataOffset;
    if( trun.m_flags & TrackFragmentRunBox::kFirstSampleFlagsPresent )
        out << " first sample flags: " << std::hex << trun.m_firstSampleFlags << std::dec;

    out << " samples (" << trun.m_sampleCount << "):\n";
    for( auto sample: trun.m_samples )
        sample.print( out );

    return out;
}

TrackFragmentRunBox::Sample::Sample( std::ifstream &f, uint32_t flags ) : m_trflags( flags ) {
    if( flags & TrackFragmentRunBox::kSampleDurationPresent ) {
        f.read( (char*)&m_sampleDuration, sizeof(m_sampleDuration) );
        m_sampleDuration = be32toh( m_sampleDuration );
    }
    if( flags & TrackFragmentRunBox::kSampleSizePresent ) {
        f.read( (char*)&m_sampleSize, sizeof(m_sampleSize) );
        m_sampleSize = be32toh( m_sampleSize );
    }
    if( flags & TrackFragmentRunBox::kSampleFlagsPresent ) {
        f.read( (char*)&m_sampleFlags, sizeof(m_sampleFlags) );
        m_sampleFlags = be32toh( m_sampleFlags );
    }
    if( flags & TrackFragmentRunBox::kSampleCompositionTimeOffsetsPresent ) {
        f.read( (char*)&m_sampleCompositionTimeOffset, sizeof(m_sampleCompositionTimeOffset) );
        m_sampleCompositionTimeOffset = be32toh( m_sampleCompositionTimeOffset );
    }
}

std::ostream& TrackFragmentRunBox::Sample::print( std::ostream &out ) {
    if( m_trflags & TrackFragmentRunBox::kSampleDurationPresent )
        out << " duration: " << m_sampleDuration << "; ";
    if( m_trflags & TrackFragmentRunBox::kSampleSizePresent )
        out << " size: " << m_sampleSize << "; ";
    if( m_trflags & TrackFragmentRunBox::kSampleFlagsPresent )
        out << " flags: " << std::hex << m_sampleFlags << std::dec<< "; ";
    if( m_trflags & TrackFragmentRunBox::kSampleCompositionTimeOffsetsPresent )
        out << " time offset: " << m_sampleCompositionTimeOffset << "; ";
    out << "\n";
}

TrackFragmentRunBox::TrackFragmentRunBox( std::ifstream& f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    tmp = be32toh( tmp );

    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    f.read( (char*)&m_sampleCount, sizeof(m_sampleCount) );
    m_sampleCount = be32toh( m_sampleCount );

    if( m_flags & kDataOffsetPresent ) {
        f.read( (char*)&m_dataOffset, sizeof(m_dataOffset) );
        m_dataOffset = be32toh( m_dataOffset );
    }
    if( m_flags & kFirstSampleFlagsPresent ) {
        f.read( (char*)&m_firstSampleFlags, sizeof(m_firstSampleFlags) );
        m_firstSampleFlags = be32toh( m_firstSampleFlags );
    }
    for( uint32_t i(0); i < m_sampleCount; ++i )
        m_samples.push_back( Sample( f, m_flags ) );
}
