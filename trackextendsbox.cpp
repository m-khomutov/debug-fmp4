#include "trackextendsbox.hh"

TrackExtendsBox::SampleFlags::SampleFlags( std::ifstream & f ) {
    uint8_t tmp = f.get();
    m_sampleDependsOn = tmp & 0x3;  // bit(6) reserved=0 + uint(2) sample_depends_on

    tmp = f.get();
    m_sampleIsDependedOn = (tmp >> 6) & 0x3;   // uint(2) sample_is_depended_on;
    m_sampleHasRedundancy = (tmp >> 4) & 0x3;  // uint(2) sample_has_redundancy;
    m_samplePaddingValue = (tmp >> 1) & 0x7;   // bit(3) sample_padding_value;
    m_sampleIsDifferenceSample = tmp & 0x1;    // bit(1) sample_is_difference_sample;
                                               // i.e. when 1 signals a non-key or non-sync sample
    f.read( (char*)&m_sampleDegradationPriority, sizeof(m_sampleDegradationPriority) );
    m_sampleDegradationPriority = be16toh( m_sampleDegradationPriority );
}

std::ostream& TrackExtendsBox::SampleFlags::print( std::ostream& out ) {
    out << std::hex;
    out << "sample_depends_on=" << int(m_sampleDependsOn);
    out << " sample_is_depended_on=" << int(m_sampleIsDependedOn);
    out << " sample_has_redundancy=" << int(m_sampleHasRedundancy);
    out << " sample_padding_value=" << int(m_samplePaddingValue);
    out << " sample_is_difference_sample=" << std::boolalpha << m_sampleIsDifferenceSample;
    out << std::dec;
    out << " sample_degradation_priority=" << m_sampleDegradationPriority;

    return out;
}

std::ostream & operator <<( std::ostream& out, const TrackExtendsBox& trex ) {
    out << "version=" << int(trex.m_version) << " flags=" << std::hex << trex.m_flags << std::dec;
    out << " track id=" << trex.m_trackID << " defaults: { sample description index=" << trex.m_defaultSampleDescriptionIndex
        << " sample duration=" << trex.m_defaultSampleDuration << " sample size=" << trex.m_defaultSampleSize << " sample flags=[";
    trex.m_defaultSampleFlags->print( out );
    out << "] }";

    return out;
}

TrackExtendsBox::TrackExtendsBox( std::ifstream & f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );

    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    f.read( (char*)&m_trackID, sizeof(m_trackID) );
    m_trackID = be32toh( m_trackID );
    f.read( (char*)&m_defaultSampleDescriptionIndex, sizeof(m_defaultSampleDescriptionIndex) );
    m_defaultSampleDescriptionIndex = be32toh( m_defaultSampleDescriptionIndex );
    f.read( (char*)&m_defaultSampleDuration, sizeof(m_defaultSampleDuration) );
    m_defaultSampleDuration = be32toh( m_defaultSampleDuration );
    f.read( (char*)&m_defaultSampleSize, sizeof(m_defaultSampleSize) );
    m_defaultSampleSize = be32toh( m_defaultSampleSize );

    m_defaultSampleFlags.reset( new SampleFlags( f ) );
}
