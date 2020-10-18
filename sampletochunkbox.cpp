#include "sampletochunkbox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const SampleToChunkBox& stsc ) {
    out << "version=" << int(stsc.m_version) << " flags=" << std::hex << stsc.m_flags << std::dec;
    out << " entries=" << stsc.m_entries.size();
    for( auto entry : stsc.m_entries ) {
        out << " { ";
        entry->print( out );
        out << " }";
    }
    return out;
}

SampleToChunkBox::Entry::Entry( std::ifstream& f ) {

    f.read( (char*)&m_firstChunk, sizeof(m_firstChunk) );
    m_firstChunk = be32toh( m_firstChunk );
    f.read( (char*)&m_samplesPerChunk, sizeof(m_samplesPerChunk) );
    m_samplesPerChunk = be32toh( m_samplesPerChunk );
    f.read( (char*)&m_sampleDescriptionIndex, sizeof(m_sampleDescriptionIndex) );
    m_sampleDescriptionIndex = be32toh( m_sampleDescriptionIndex );
}

std::ostream& SampleToChunkBox::Entry::print( std::ostream& out ) {
    out << "first chunk=" << m_firstChunk << " samples per chunk=" << m_samplesPerChunk << " sample description index=" << m_sampleDescriptionIndex;
    return out;
}

SampleToChunkBox::SampleToChunkBox( std::ifstream & f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    f.read( (char*)&tmp, sizeof(tmp) );
    tmp = be32toh( tmp );
    for( uint32_t i(0); i < tmp; ++i )
        m_entries.push_back( std::shared_ptr< Entry >(new Entry( f )) );
}
