#include "trackextendsbox.hh"

TrackExtendsBox::SampleFlags::SampleFlags( std::istream & is ) {
    uint8_t tmp = is.get();
    m_sampleDependsOn = tmp & 0x3;  // bit(6) reserved=0 + uint(2) sample_depends_on

    tmp = is.get();
    m_sampleIsDependedOn = (tmp >> 6) & 0x3;   // uint(2) sample_is_depended_on;
    m_sampleHasRedundancy = (tmp >> 4) & 0x3;  // uint(2) sample_has_redundancy;
    m_samplePaddingValue = (tmp >> 1) & 0x7;   // bit(3) sample_padding_value;
    m_sampleIsDifferenceSample = tmp & 0x1;    // bit(1) sample_is_difference_sample;
                                               // i.e. when 1 signals a non-key or non-sync sample
    is.read( (char*)&m_sampleDegradationPriority, sizeof(m_sampleDegradationPriority) );
    m_sampleDegradationPriority = be16toh( m_sampleDegradationPriority );
}

void TrackExtendsBox::SampleFlags::fout( std::ostream& out ) {
    out << std::hex;
    out << "sample_depends_on=" << int(m_sampleDependsOn);
    out << " sample_is_depended_on=" << int(m_sampleIsDependedOn);
    out << " sample_has_redundancy=" << int(m_sampleHasRedundancy);
    out << " sample_padding_value=" << int(m_samplePaddingValue);
    out << " sample_is_difference_sample=" << std::boolalpha << m_sampleIsDifferenceSample;
    out << std::dec;
    out << " sample_degradation_priority=" << m_sampleDegradationPriority;
}

TrackExtendsBox::TrackExtendsBox( std::istream & is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );

    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    is.read( (char*)&m_trackID, sizeof(m_trackID) );
    m_trackID = be32toh( m_trackID );
    is.read( (char*)&m_defaultSampleDescriptionIndex, sizeof(m_defaultSampleDescriptionIndex) );
    m_defaultSampleDescriptionIndex = be32toh( m_defaultSampleDescriptionIndex );
    is.read( (char*)&m_defaultSampleDuration, sizeof(m_defaultSampleDuration) );
    m_defaultSampleDuration = be32toh( m_defaultSampleDuration );
    is.read( (char*)&m_defaultSampleSize, sizeof(m_defaultSampleSize) );
    m_defaultSampleSize = be32toh( m_defaultSampleSize );

    m_defaultSampleFlags.reset( new SampleFlags( is ) );
}

void TrackExtendsBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "\nversion=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " track id=" << m_trackID << " defaults: { sample description index=" << m_defaultSampleDescriptionIndex
        << " sample duration=" << m_defaultSampleDuration << " sample size=" << m_defaultSampleSize << " sample flags=[";
    m_defaultSampleFlags->fout( out );
    out << "] }";
}
