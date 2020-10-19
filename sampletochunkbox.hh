#ifndef SAMPLETOCHUNK_HH
#define SAMPLETOCHUNK_HH

#include "atom.h"
#include <vector>
#include <memory>

class SampleToChunkBox : public Atom {
public:
    class Entry {
    public:
        Entry( std::istream& is );

        void fout( std::ostream& out );

        uint32_t firstChunk() const {
            return m_firstChunk;
        }
        uint32_t samplesPerChunk() const {
            return m_samplesPerChunk;
        }
        uint32_t sampleDescriptionIndex() const {
            return m_sampleDescriptionIndex;
        }

    private:
        uint32_t m_firstChunk;
        uint32_t m_samplesPerChunk;
        uint32_t m_sampleDescriptionIndex;
    };

    SampleToChunkBox( std::istream & is );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::shared_ptr< Entry > > m_entries;

private:
    void fout( std::ostream& out ) const override;
};

#endif // SAMPLETOCHUNK_HH
