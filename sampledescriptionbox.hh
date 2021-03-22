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

class TimedTextSampleEntry : public SampleEntry {
public:
    struct StyleRecord {
        uint16_t startChar { 0 };
        uint16_t endChar { 0 };
        uint16_t fontID;

        enum Style { Plain, Bold, Italic, Underline };
        uint8_t face_style_flags { Plain };

        uint8_t font_size;
        uint8_t text_color_rgba[4] { 0,0,0,0 };

        StyleRecord( uint16_t fid = 0, uint8_t fsz = 12 ) : fontID( fid ),font_size( fsz ) {}
    };
    struct FontRecord {
        uint16_t fontID;
        std::string font;
    };
    struct BoxRecord {
        int16_t top;
        int16_t left;
        int16_t bottom;
        int16_t right;
    };
    class FontTableBox : public Atom {
    public:
        FontTableBox( std::istream& f );

    protected:
        void fout( std::ostream& out ) const override;

    private:
        std::vector< FontRecord > m_fontEntries;
    };

    TimedTextSampleEntry( std::istream& f );

private:
    uint32_t m_displayFlags;
    int8_t m_horizontalJustification;
    int8_t m_verticalJustification;
    uint8_t m_backgroundColorRgba[4];
    BoxRecord m_defaultTextBox;
    StyleRecord m_defaultStyle;
    std::unique_ptr< FontTableBox > m_fontTable;

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
