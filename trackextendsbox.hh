#ifndef TRACKEXTENDSBOX_HH
#define TRACKEXTENDSBOX_HH

#include "atom.h"
#include <memory>

class TrackExtendsBox: public Atom {
public:
    class SampleFlags {
    public:
        SampleFlags( std::istream& is );

        void fout( std::ostream& out );

    private:
        uint8_t m_sampleDependsOn;
        uint8_t m_sampleIsDependedOn;
        uint8_t m_sampleHasRedundancy;
        uint8_t m_samplePaddingValue;

        bool m_sampleIsDifferenceSample;

        uint16_t m_sampleDegradationPriority;
    };

    TrackExtendsBox( std::istream & is );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint32_t m_trackID;
    uint32_t m_defaultSampleDescriptionIndex;
    uint32_t m_defaultSampleDuration;
    uint32_t m_defaultSampleSize;
    std::shared_ptr< SampleFlags > m_defaultSampleFlags;

private:
    void fout( std::ostream& out ) const override;
};

#endif // TRACKEXTENDSBOX_HH
