//
// Created by mkh on 27.01.2021.
//

#include "initialobjectdescriptorbox.hh"
#include <iostream>

InitialObjectDescriptorBox::InitialObjectDescriptorBox( std::istream & is, uint32_t sz ) : Atom( is ),m_data( sz - 8 ) {
    is.read( (char*)m_data.data(), m_data.size() );
    //is.seekg( p + std::streampos(size() - 8) );
}

void InitialObjectDescriptorBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "[ ";
    for( auto c : m_data )
      out << std::hex << int(c) << " ";
    out << "]";
}
