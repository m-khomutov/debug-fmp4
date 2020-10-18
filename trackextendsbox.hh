#ifndef TRACKEXTENDSBOX_HH
#define TRACKEXTENDSBOX_HH

#include <fstream>
#include <memory>

class TrackExtendsBox
{
public:
    class SampleFlags {
    public:
        SampleFlags( std::ifstream& f );

        std::ostream& print( std::ostream& out );

    private:
        uint8_t m_sampleDependsOn;
        uint8_t m_sampleIsDependedOn;
        uint8_t m_sampleHasRedundancy;
        uint8_t m_samplePaddingValue;

        bool m_sampleIsDifferenceSample;

        uint16_t m_sampleDegradationPriority;
    };

    TrackExtendsBox( std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint32_t m_trackID;
    uint32_t m_defaultSampleDescriptionIndex;
    uint32_t m_defaultSampleDuration;
    uint32_t m_defaultSampleSize;
    std::shared_ptr< SampleFlags > m_defaultSampleFlags;

    friend std::ostream & operator <<( std::ostream& out, const TrackExtendsBox& trex );
};

#endif // TRACKEXTENDSBOX_HH
