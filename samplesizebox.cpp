#include "samplesizebox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const SampleSizeBox& stsz ) {
    out << "version=" << int(stsz.m_version) << " flags=" << std::hex << stsz.m_flags << std::dec;
    out << " samplesize=" << stsz.m_samplesize << " samplecount=" << stsz.m_samplecount;

    return out;
}

SampleSizeBox::SampleSizeBox( std::ifstream & f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    f.read( (char*)&m_samplesize, sizeof(m_samplesize) );
    m_samplesize = be32toh( m_samplesize );
    f.read( (char*)&m_samplecount, sizeof(m_samplecount) );
    m_samplecount = be32toh( m_samplecount );
    if( m_samplesize == 0 ) {
        for( uint32_t i(0); i < m_samplecount; ++i ) {
            uint32_t tmp;
            f.read( (char*)&tmp, sizeof(tmp) );
            m_entrysizeVector.push_back( be32toh( tmp ) );
        }
    }
}
