#include "moviefragmentheaderbox.hh"

std::ostream & operator <<( std::ostream& out, const MovieFragmentHeaderBox& mfhd ) {
    out << "version=" << int(mfhd.m_version) << " flags=" << std::hex << mfhd.m_flags << std::dec << " sn: " << mfhd.m_sequenceNumber;
    return out;
}
MovieFragmentHeaderBox::MovieFragmentHeaderBox( std::ifstream & f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    f.read( (char*)&m_sequenceNumber, sizeof(m_sequenceNumber) );
    m_sequenceNumber = be32toh( m_sequenceNumber );
}
