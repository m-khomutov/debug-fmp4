#include "sampledescriptionbox.hh"
#include <iostream>

SampleEntry::SampleEntry( std::istream& is, const std::string & fmt ) : Atom( is ), m_format( fmt ) {
    for( int i(0); i < 6; ++i )  //  unsigned int(8)[6] reserved = 0;
        is.get();
    is.read( (char*)&m_dataReferenceIndex, sizeof(m_dataReferenceIndex) );
    m_dataReferenceIndex = be16toh( m_dataReferenceIndex );
}

void SampleEntry::fout( std::ostream& out ) const {
    out << "   ";
    Atom::fout( out );
    out << "format='" << m_format << "' data reference index=" << m_dataReferenceIndex;
}

HintSampleEntry::HintSampleEntry( std::istream& is ) : SampleEntry( is, "hint" ) {

    m_data.resize( size() - (2 * sizeof(uint32_t) + /*SampleEntry fields*/8) );
    is.read( (char*)m_data.data(), m_data.size() );
}

void HintSampleEntry::fout( std::ostream& out ) const {
    SampleEntry::fout( out );

    out << std::hex << " data=[ ";
    for( auto b : m_data )
        out << int(b) << " ";
    out << std::dec << "]";
}

VisualSampleEntry::VisualSampleEntry( std::istream& is ) : SampleEntry ( is, "vide" ) {
    uint16_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );  // uint(16) pre_defined = 0;
    is.read( (char*)&tmp, sizeof(tmp) );  // unsigned int(16) reserved = 0;
    for( int i(0); i < 6; ++i )
        is.read( (char*)&tmp, sizeof(tmp) );  // unsigned int(32)[3] pre_defined = 0;

    is.read( (char*)&m_width, sizeof(m_width) );
    m_width = be16toh( m_width );
    is.read( (char*)&m_height, sizeof(m_height) );
    m_height = be16toh( m_height );
    is.read( (char*)&m_horizresolution, sizeof(m_horizresolution) );
    is.read( (char*)&m_vertresolution, sizeof(m_vertresolution) );

    for( int i(0); i < 2; ++i )
        is.read( (char*)&tmp, sizeof(tmp) );  // unsigned int(32) reserved = 0;

    is.read( (char*)&m_frameCount, sizeof(m_frameCount) );
    m_frameCount = be16toh( m_frameCount );
    is.read( (char*)m_compressorname, sizeof(m_compressorname)-1 );
    m_compressorname[32] = 0;
    is.read( (char*)&m_depth, sizeof(m_depth) );
    m_depth = be16toh( m_depth );
    is.read( (char*)&tmp, sizeof(tmp) );  // int(16) pre_defined = -1;
}

void VisualSampleEntry::fout( std::ostream& out ) const {
    SampleEntry::fout( out );
    out << " width=" << m_width << " height=" << m_height << std::hex << " hresolution=0x" << m_horizresolution
        << " vresolution=0x" << m_vertresolution << std::dec << " frame count=" << m_frameCount << " name='" << m_compressorname
        << "' depth=" << m_depth;
}



AudioSampleEntry::AudioSampleEntry( std::istream& is ) : SampleEntry ( is, "soun" )
{
    uint32_t tmp;
    for( int i(0); i < 2; ++i )
        is.read( (char*)&tmp, sizeof(tmp) );  // unsigned int(16) reserved = 0;
    is.read( (char*)&m_channelcount, sizeof(m_channelcount) );
    m_channelcount = be16toh( m_channelcount );
    is.read( (char*)&m_samplesize, sizeof(m_samplesize) );
    m_samplesize = be16toh( m_samplesize );

    is.read( (char*)&tmp, sizeof(uint16_t) );  // uint(16) pre_defined = 0;
    is.read( (char*)&tmp, sizeof(uint16_t) );  // unsigned int(16) reserved = 0;

    is.read( (char*)&m_samplerate, sizeof(m_samplerate) );
    m_samplerate = (be32toh( m_samplerate )) >> 16;
}

void AudioSampleEntry::fout( std::ostream& out ) const {
    SampleEntry::fout( out );
    out << " channels=" << m_channelcount << " samplesize=" << m_samplesize <<  " samplerate=" << m_samplerate;
}


TimedTextSampleEntry::FontTableBox::FontTableBox(std::istream &f)
: Atom( f ) {
    uint16_t count;
    f.read( (char*)&count, sizeof(count) );
    count = be16toh( count );

    if( count ) {
        m_fontEntries.resize( count );
        for( uint16_t i(0); i < count; ++i ) {
            f.read( (char*)&m_fontEntries[i].fontID, sizeof(m_fontEntries[i].fontID) );
            m_fontEntries[i].fontID = be16toh( m_fontEntries[i].fontID );
            m_fontEntries[i].font.resize( f.get() );
            f.read( &m_fontEntries[i].font[0], m_fontEntries[i].font.size() );
        }
    }
}
void TimedTextSampleEntry::FontTableBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << " entries=[ ";
    for( auto entry : m_fontEntries )
        out << "{ " << "id=" << entry.fontID << ": " << entry.font << " }";
    out << " ]";
}

TimedTextSampleEntry::TimedTextSampleEntry( std::istream& is ) : SampleEntry( is, "text" ) {
    is.read( (char*)&m_displayFlags, sizeof(m_displayFlags) );
    m_horizontalJustification = is.get();
    m_verticalJustification = is.get();
    is.read( (char*)m_backgroundColorRgba, sizeof(m_backgroundColorRgba) );

    is.read( (char*)&m_defaultTextBox.top, sizeof(m_defaultTextBox.top) );
    m_defaultTextBox.top = be16toh( m_defaultTextBox.top );
    is.read( (char*)&m_defaultTextBox.left, sizeof(m_defaultTextBox.left) );
    m_defaultTextBox.left = be16toh( m_defaultTextBox.left );
    is.read( (char*)&m_defaultTextBox.bottom, sizeof(m_defaultTextBox.bottom) );
    m_defaultTextBox.bottom = be16toh( m_defaultTextBox.bottom );
    is.read( (char*)&m_defaultTextBox.right, sizeof(m_defaultTextBox.right) );
    m_defaultTextBox.right = be16toh( m_defaultTextBox.right );

    is.read( (char*)&m_defaultStyle.startChar, sizeof(m_defaultStyle.startChar) );
    m_defaultStyle.startChar = be16toh( m_defaultStyle.startChar );
    is.read( (char*)&m_defaultStyle.endChar, sizeof(m_defaultStyle.endChar) );
    m_defaultStyle.endChar = be16toh( m_defaultStyle.endChar );
    is.read( (char*)&m_defaultStyle.fontID, sizeof(m_defaultStyle.fontID) );
    m_defaultStyle.fontID = be16toh( m_defaultStyle.fontID );

    m_defaultStyle.face_style_flags = is.get();
    m_defaultStyle.font_size = is.get();
    is.read( (char*)&m_defaultStyle.text_color_rgba, sizeof(m_defaultStyle.text_color_rgba) );
    m_fontTable.reset( new FontTableBox( is ) );
}

void TimedTextSampleEntry::fout( std::ostream& out ) const {
  SampleEntry::fout( out );
  out << " displayFlags=0x" << std::hex << m_displayFlags << std::dec
      << " horizontal-justification=" << int(m_horizontalJustification)
      << " vertical-justification=" << int(m_verticalJustification)
      << " background-color-rgba=[ " << int(m_backgroundColorRgba[0]) << ","
                                     << int(m_backgroundColorRgba[1]) << ","
                                     << int(m_backgroundColorRgba[2]) << ","
                                     << int(m_backgroundColorRgba[3]) << " ]"
      << " default-text-box=[ " << m_defaultTextBox.top << " "
                                << m_defaultTextBox.left << " "
                                << m_defaultTextBox.bottom << " "
                                << m_defaultTextBox.right << " ]"
      << " startChar=" << m_defaultStyle.startChar << " endChar=" << m_defaultStyle.endChar
      << " font-ID=" << m_defaultStyle.fontID << " face-style-flags={";
      if( m_defaultStyle.face_style_flags == StyleRecord::Style::Plain )
          out << " plain";
      if( m_defaultStyle.face_style_flags & StyleRecord::Style::Bold )
          out << " bold";
      if( m_defaultStyle.face_style_flags & StyleRecord::Style::Italic )
          out << " italic";
      if( m_defaultStyle.face_style_flags & StyleRecord::Style::Underline )
          out << "underline";
      out << " } font-size=" << int(m_defaultStyle.font_size) << " text-color-rgba=[ "
          << int(m_defaultStyle.text_color_rgba[0]) << ","
          << int(m_defaultStyle.text_color_rgba[1]) << ","
          << int(m_defaultStyle.text_color_rgba[2]) << ","
          << int(m_defaultStyle.text_color_rgba[3]) << " ]";
      if( m_fontTable.get() )
          out << " FontTableBox={ " << *m_fontTable << " }";
}



std::ostream & operator <<( std::ostream& out, const SampleDescriptionBox& stsd ) {
   return out;
}

SampleDescriptionBox::SampleDescriptionBox( std::istream& is, const std::string& handler_type ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    uint32_t count;
    is.read( (char*)&count, sizeof(count) );
    count = be32toh( count );

    for( uint32_t i(0); i < count; ++i ) {
        if( handler_type == "vide" ) {
            m_entries.push_back( std::shared_ptr< SampleEntry >(new VisualSampleEntry( is )) );
        }
        else if( handler_type == "soun" ) {
            m_entries.push_back( std::shared_ptr< SampleEntry >(new AudioSampleEntry( is )) );
        }
        else if( handler_type == "hint" ) {
            m_entries.push_back( std::shared_ptr< SampleEntry >(new HintSampleEntry( is )) );
        }
        else if( handler_type == "text" ) {
            m_entries.push_back( std::shared_ptr< SampleEntry >(new TimedTextSampleEntry( is )) );
        }
    }
}

void SampleDescriptionBox::fout( std::ostream& out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec << " entries=" << m_entries.size();
    for( auto entry : m_entries ) {
        out << "\n";
        indent( out );
        entry->fout( out );
    }
}
