#ifndef SAMPLEDESCRIPTIONBOX_HH
#define SAMPLEDESCRIPTIONBOX_HH

#include <fstream>
#include <vector>
#include <memory>

class SampleEntry {
public:
    SampleEntry( std::ifstream& f, uint32_t sz );
    virtual ~SampleEntry() = default;

    virtual std::ostream& print( std::ostream& out ) = 0;

    uint32_t size() const {
        return m_size;
    }

protected:
    uint32_t m_size{0};
    std::string m_format;

    uint16_t m_dataReferenceIndex;
};


class HintSampleEntry : public SampleEntry {
public:
    HintSampleEntry( std::ifstream& f, uint32_t sz );

    std::ostream& print( std::ostream& out ) override;

private:
    std::vector< uint8_t > m_data;
};


class VisualSampleEntry : public SampleEntry {
public:
    VisualSampleEntry( std::ifstream& f, uint32_t sz );

    std::ostream& print( std::ostream& out ) override;

private:
    uint16_t m_width;
    uint16_t m_height;

    uint32_t m_horizresolution{0x00480000}; // 72 dpi
    uint32_t m_vertresolution{0x00480000}; // 72 dpi

    uint16_t m_frameCount{1};
    char m_compressorname[ 33 ];
    uint16_t m_depth{0x0018};
};


class AudioSampleEntry : public SampleEntry {
public:
    AudioSampleEntry( std::ifstream& f, uint32_t sz );

    std::ostream& print( std::ostream& out ) override;

private:
    uint16_t m_channelcount;
    uint16_t m_samplesize;
    uint32_t m_samplerate;
};



class SampleDescriptionBox
{
public:
    SampleDescriptionBox( std::ifstream& f, uint32_t sz, const std::string& handler_type );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::shared_ptr< SampleEntry > > m_entries;

    friend std::ostream & operator <<( std::ostream& out, const SampleDescriptionBox& stsd );
};

#endif // SAMPLEDESCRIPTIONBOX_HH
