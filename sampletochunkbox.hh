#ifndef SAMPLETOCHUNK_HH
#define SAMPLETOCHUNK_HH

#include <fstream>
#include <vector>
#include <memory>

class SampleToChunkBox
{
public:
    class Entry {
    public:
        Entry( std::ifstream& f );

        std::ostream& print( std::ostream& out );

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

    SampleToChunkBox( std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::shared_ptr< Entry > > m_entries;

    friend std::ostream & operator <<( std::ostream& out, const SampleToChunkBox& mvhd );
};

#endif // SAMPLETOCHUNK_HH
