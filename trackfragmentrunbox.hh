#ifndef TRACKFRAGMENTRUNBOX_HH
#define TRACKFRAGMENTRUNBOX_HH

#include "atom.h"

#include <vector>
#include <map>
#include <memory>

class TrackFragmentRunBox : public Atom {
public:
    enum TrackRunFlags {
        kDataOffsetPresent = 0x000001,
        kFirstSampleFlagsPresent = 0x000004,
        kSampleDurationPresent = 0x000100,
        kSampleSizePresent = 0x000200,
        kSampleFlagsPresent = 0x000400,
        kSampleCompositionTimeOffsetsPresent = 0x000800
    };

    class Sample {
    public:
        Sample() = default;
        Sample( std::istream& is, uint32_t flags );

        uint32_t trFlags() const {
            return m_trflags;
	}
        uint32_t duration() const {
            return m_sampleDuration;
	}
	uint32_t size() const {
            return m_sampleSize;
	}
	uint32_t flags() const {
            return m_sampleFlags;
	}
	uint32_t compositionTimeOffset() const {
            return m_sampleCompositionTimeOffset;
	}

    void fout( std::ostream& out );

    private:
        uint32_t m_trflags;
        uint32_t m_sampleDuration;
        uint32_t m_sampleSize;
        uint32_t m_sampleFlags;
        uint32_t m_sampleCompositionTimeOffset;
    };

    TrackFragmentRunBox( std::istream & is );

    const Sample & operator[]( int at ) {
        return m_samples[at];
    }

    const std::vector< Sample >::iterator begin() {
        return m_samples.begin();
    }

    const std::vector< Sample >::iterator end() {
        return m_samples.end();
    }

    uint32_t dataoffset() const {
        return m_dataOffset;
    }

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint32_t m_sampleCount;
    // the following are optional fields
    int32_t m_dataOffset;
    uint32_t m_firstSampleFlags;
    // all fields in the following array are optional
    std::vector< Sample > m_samples;

private:
    void fout( std::ostream& out ) const override;
};
using TrunMap = std::map< uint32_t/*trackID*/, std::shared_ptr< TrackFragmentRunBox > >;

#endif // TRACKFRAGMENTRUNBOX_HH
