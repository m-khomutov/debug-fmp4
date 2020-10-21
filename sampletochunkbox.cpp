#include "sampletochunkbox.hh"

SampleToChunkBox::Entry::Entry( std::istream& is ) {

    is.read( (char*)&m_firstChunk, sizeof(m_firstChunk) );
    m_firstChunk = be32toh( m_firstChunk );
    is.read( (char*)&m_samplesPerChunk, sizeof(m_samplesPerChunk) );
    m_samplesPerChunk = be32toh( m_samplesPerChunk );
    is.read( (char*)&m_sampleDescriptionIndex, sizeof(m_sampleDescriptionIndex) );
    m_sampleDescriptionIndex = be32toh( m_sampleDescriptionIndex );
}

void SampleToChunkBox::Entry::fout( std::ostream& out ) {
    out << m_firstChunk << ":" << m_samplesPerChunk << ":" << m_sampleDescriptionIndex << " ";
}

SampleToChunkBox::SampleToChunkBox( std::istream & is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    is.read( (char*)&tmp, sizeof(tmp) );
    tmp = be32toh( tmp );
    for( uint32_t i(0); i < tmp; ++i )
        m_entries.push_back( std::shared_ptr< Entry >(new Entry( is )) );
}

void SampleToChunkBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " entries(first:samples:index)={ " << m_entries.size();
    for( auto entry : m_entries )
        entry->fout( out );
    out << "}";
}
