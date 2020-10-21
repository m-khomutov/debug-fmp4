//
// Created by mkh on 19.10.2020.
//

#include "freebox.hh"

Free::Free( std::istream & is, uint32_t sz ) : Atom( is ),m_content_size( sz - 8 ) {
    if( m_content_size )
        is.seekg( is.tellg() + std::streampos(m_content_size) );
}

void Free::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "contents size=" << m_content_size;
}
