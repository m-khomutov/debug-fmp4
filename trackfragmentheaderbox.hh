#ifndef TRACKFRAGMENTHEADERBOX_HH
#define TRACKFRAGMENTHEADERBOX_HH

#include "atom.h"
#include "trackextendsbox.hh"

#include <memory>

class TrackFragmentHeaderBox : public Atom {
public:
    enum {
        kBaseDataOffsetPresent         = 0x000001,
        kSampleDescriptionIndexPresent = 0x000002,
        kDefaultSampleDurationPresent  = 0x000008,
        kDefaultSampleSizePresent      = 0x000010,
        kDefaultSampleFlagsPresent     = 0x000020,
        kDurationIsEmpty               = 0x010000,
        kDefaultBaseIsMoof             = 0x020000
    };

    TrackFragmentHeaderBox( std::istream & is );

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

private:
    void fout( std::ostream& out ) const override;
};

#endif // TRACKFRAGMENTHEADERBOX_HH
