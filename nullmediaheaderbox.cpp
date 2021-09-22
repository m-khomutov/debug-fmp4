//
// Created by mkh on 17.03.2021.
//

#include "nullmediaheaderbox.hh"

NullMediaHeaderBox::NullMediaHeaderBox( std::istream& is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;
}

void NullMediaHeaderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
}
