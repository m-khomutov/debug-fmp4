#include "sampledescriptionbox.hh"

SampleEntry::SampleEntry( std::istream& is, const std::string & fmt ) : Atom( is ), m_format( fmt ) {
    for( int i(0); i < 6; ++i )  //  unsigned int(8)[6] reserved = 0;
        is.get();
    is.read( (char*)&m_dataReferenceIndex, sizeof(m_dataReferenceIndex) );
    m_dataReferenceIndex = be16toh( m_dataReferenceIndex );
}

void SampleEntry::fout( std::ostream& out ) const {
    out << " size=" << size() << " format='" << m_format << "' data reference index=" << m_dataReferenceIndex;
}

HintSampleEntry::HintSampleEntry( std::istream& is ) : SampleEntry( is, "hint" ) {

    m_data.resize( size() - 2 * sizeof(uint32_t) );
    is.read( (char*)m_data.data(), m_data.size() );
}

void HintSampleEntry::fout( std::ostream& out ) const {
    SampleEntry::fout( out );

    out << std::hex;
    for( auto b : m_data )
        out << b << " ";
    out << std::dec;
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
    }
}

void SampleDescriptionBox::fout( std::ostream& out ) const {
    Atom::fout( out );
    out << "\nversion=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec << " entries=" << m_entries.size();
    int it{0};
    for( auto entry : m_entries ) {
        out << "\n\t" << it << ". ";
        entry->fout( out );
    }
}
