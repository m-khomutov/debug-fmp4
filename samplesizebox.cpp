#include "samplesizebox.hh"

#include <iostream>

SampleSizeBox::SampleSizeBox( std::istream& is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    is.read( (char*)&m_samplesize, sizeof(m_samplesize) );
    m_samplesize = be32toh( m_samplesize );
    is.read( (char*)&m_samplecount, sizeof(m_samplecount) );
    m_samplecount = be32toh( m_samplecount );
    if( m_samplesize == 0 ) {
        for( uint32_t i(0); i < m_samplecount; ++i ) {
            uint32_t tmp;
            is.read( (char*)&tmp, sizeof(tmp) );
            m_entrysizeVector.push_back( be32toh( tmp ) );
        }
    }
}

void SampleSizeBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "\nversion=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " samplesize=" << m_samplesize << " samplecount=" << m_samplecount;
}