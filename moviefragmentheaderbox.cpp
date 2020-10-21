#include "moviefragmentheaderbox.hh"

MovieFragmentHeaderBox::MovieFragmentHeaderBox( std::istream & is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    is.read( (char*)&m_sequenceNumber, sizeof(m_sequenceNumber) );
    m_sequenceNumber = be32toh( m_sequenceNumber );
}

void MovieFragmentHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec << " sn: " << m_sequenceNumber;
}
