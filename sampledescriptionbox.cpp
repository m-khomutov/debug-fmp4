#include "sampledescriptionbox.hh"

#include <iostream>

SampleEntry::SampleEntry( std::ifstream& f, uint32_t sz )
{
    // Box=size+type;
    f.read( (char*)&m_size, sizeof(m_size) );
    m_size = be32toh( m_size );
    char tmp[ 4 ];
    f.read( (char*)&tmp, sizeof(tmp) );
    m_format = std::string( tmp, sizeof(tmp) );

    for( int i(0); i < 6; ++i )  //  unsigned int(8)[6] reserved = 0;
        f.get();
    f.read( (char*)&m_dataReferenceIndex, sizeof(m_dataReferenceIndex) );
    m_dataReferenceIndex = be16toh( m_dataReferenceIndex );
}

std::ostream& SampleEntry::print( std::ostream& out ) {
    out << " size=" << m_size << " format='" << m_format << "' data reference index=" << m_dataReferenceIndex;
    return out;
}

HintSampleEntry::HintSampleEntry( std::ifstream& f, uint32_t sz )
    : SampleEntry( f, sz ) {

    m_data.resize( sz - 2*sizeof(uint32_t) );
    f.read( (char*)m_data.data(), m_data.size() );
}

std::ostream& HintSampleEntry::print( std::ostream& out ) {
    SampleEntry::print( out );

    out << std::hex;
    for( auto b : m_data )
        out << b << " ";
    out << std::dec;

    return out;
}

VisualSampleEntry::VisualSampleEntry( std::ifstream& f, uint32_t sz )
    : SampleEntry ( f, sz )
{
    uint16_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );  // uint(16) pre_defined = 0;
    f.read( (char*)&tmp, sizeof(tmp) );  // unsigned int(16) reserved = 0;
    for( int i(0); i < 6; ++i )
        f.read( (char*)&tmp, sizeof(tmp) );  // unsigned int(32)[3] pre_defined = 0;

    f.read( (char*)&m_width, sizeof(m_width) );
    m_width = be16toh( m_width );
    f.read( (char*)&m_height, sizeof(m_height) );
    m_height = be16toh( m_height );
    f.read( (char*)&m_horizresolution, sizeof(m_horizresolution) );
    f.read( (char*)&m_vertresolution, sizeof(m_vertresolution) );

    for( int i(0); i < 2; ++i )
        f.read( (char*)&tmp, sizeof(tmp) );  // unsigned int(32) reserved = 0;

    f.read( (char*)&m_frameCount, sizeof(m_frameCount) );
    m_frameCount = be16toh( m_frameCount );
    f.read( (char*)m_compressorname, sizeof(m_compressorname)-1 );
    m_compressorname[32] = 0;
    f.read( (char*)&m_depth, sizeof(m_depth) );
    m_depth = be16toh( m_depth );
    f.read( (char*)&tmp, sizeof(tmp) );  // int(16) pre_defined = -1;

}

std::ostream& VisualSampleEntry::print( std::ostream& out ) {
    SampleEntry::print( out );
    out << " width=" << m_width << " height=" << m_height << std::hex << " hresolution=0x" << m_horizresolution
        << " vresolution=0x" << m_vertresolution << std::dec << " frame count=" << m_frameCount << " name='" << m_compressorname
        << "' depth=" << m_depth;
    return out;
}



AudioSampleEntry::AudioSampleEntry( std::ifstream& f, uint32_t sz )
    : SampleEntry ( f, sz )
{
    uint32_t tmp;
    for( int i(0); i < 2; ++i )
        f.read( (char*)&tmp, sizeof(tmp) );  // unsigned int(16) reserved = 0;
    f.read( (char*)&m_channelcount, sizeof(m_channelcount) );
    m_channelcount = be16toh( m_channelcount );
    f.read( (char*)&m_samplesize, sizeof(m_samplesize) );
    m_samplesize = be16toh( m_samplesize );

    f.read( (char*)&tmp, sizeof(uint16_t) );  // uint(16) pre_defined = 0;
    f.read( (char*)&tmp, sizeof(uint16_t) );  // unsigned int(16) reserved = 0;

    f.read( (char*)&m_samplerate, sizeof(m_samplerate) );
    m_samplerate = (be32toh( m_samplerate )) >> 16;
}

std::ostream& AudioSampleEntry::print( std::ostream& out ) {
    SampleEntry::print( out );
    out << " channels=" << m_channelcount << " samplesize=" << m_samplesize <<  " samplerate=" << m_samplerate;
    return out;
}



std::ostream & operator <<( std::ostream& out, const SampleDescriptionBox& stsd ) {
    out << "version=" << int(stsd.m_version) << " flags=" << std::hex << stsd.m_flags << std::dec << " entries=" << stsd.m_entries.size();

    int it{0};
    for( auto entry : stsd.m_entries ) {
        out << "\n\t" << it << ". ";
        entry->print( out );
    }
    return out;
}

SampleDescriptionBox::SampleDescriptionBox( std::ifstream& f, uint32_t sz, const std::string& handler_type )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    uint32_t count;
    f.read( (char*)&count, sizeof(count) );
    count = be32toh( count );

    for( uint32_t i(0); i < count; ++i ) {
        if( handler_type == "vide" ) {
            m_entries.push_back( std::shared_ptr< SampleEntry >(new VisualSampleEntry( f, sz )) );
        }
        else if( handler_type == "soun" ) {
            m_entries.push_back( std::shared_ptr< SampleEntry >(new AudioSampleEntry( f, sz )) );
        }
        else if( handler_type == "hint" ) {
            m_entries.push_back( std::shared_ptr< SampleEntry >(new HintSampleEntry( f, sz )) );
        }
    }
}
