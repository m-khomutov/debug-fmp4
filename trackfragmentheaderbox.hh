#ifndef TRACKFRAGMENTHEADERBOX_HH
#define TRACKFRAGMENTHEADERBOX_HH

#include "trackextendsbox.hh"

#include <fstream>
#include <memory>

class TrackFragmentHeaderBox
{
public:
    enum {
        kBaseDataOffsetPresent         = 0x000001,
        kSampleDescriptionIndexPresent = 0x000002,
        kDefaultSampleDurationPresent  = 0x000008,
        kDefaultSampleSizePresent      = 0x000010,
        kDefaultSampleFlagsPresent     = 0x000020,
        kDurationIsEmpty               = 0x010000
    };

    TrackFragmentHeaderBox( std::ifstream & f, uint32_t sz );

    uint32_t trackID() const {
        return m_trackID;
    }

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint32_t m_trackID;
    // all the following are optional fields
    uint64_t m_baseDataOffset;
    uint32_t m_sampleDescriptionIndex;
    uint32_t m_defaultSampleDuration;
    uint32_t m_defaultSampleSize;
    std::shared_ptr< TrackExtendsBox::SampleFlags > m_defaultSampleFlags;

    friend std::ostream & operator <<( std::ostream& out, const TrackFragmentHeaderBox& mvhd );
};

#endif // TRACKFRAGMENTHEADERBOX_HH
