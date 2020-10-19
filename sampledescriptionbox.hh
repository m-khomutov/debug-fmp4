#ifndef SAMPLEDESCRIPTIONBOX_HH
#define SAMPLEDESCRIPTIONBOX_HH

#include "atom.h"
#include <vector>
#include <memory>

class SampleEntry : public Atom {
public:
    SampleEntry( std::istream& f, const std::string & fmt );
    virtual ~SampleEntry() = default;

protected:
    std::string m_format;
    uint16_t m_dataReferenceIndex;

protected:
    void fout( std::ostream& out ) const override;

    friend class SampleDescriptionBox;
};


class HintSampleEntry : public SampleEntry {
public:
    HintSampleEntry( std::istream& f );

private:
    std::vector< uint8_t > m_data;

private:
    void fout( std::ostream& out ) const override;
};


class VisualSampleEntry : public SampleEntry {
public:
    VisualSampleEntry( std::istream& f );

private:
    uint16_t m_width;
    uint16_t m_height;

    uint32_t m_horizresolution{0x00480000}; // 72 dpi
    uint32_t m_vertresolution{0x00480000}; // 72 dpi

    uint16_t m_frameCount{1};
    char m_compressorname[ 33 ];
    uint16_t m_depth{0x0018};

private:
    void fout( std::ostream& out ) const override;
};


class AudioSampleEntry : public SampleEntry {
public:
    AudioSampleEntry( std::istream& f );

private:
    uint16_t m_channelcount;
    uint16_t m_samplesize;
    uint32_t m_samplerate;

private:
    void fout( std::ostream& out ) const override;
};



class SampleDescriptionBox : public Atom {
public:
    SampleDescriptionBox( std::istream& f, const std::string& handler_type );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::shared_ptr< SampleEntry > > m_entries;

private:
    void fout( std::ostream& out ) const override;
};

#endif // SAMPLEDESCRIPTIONBOX_HH
