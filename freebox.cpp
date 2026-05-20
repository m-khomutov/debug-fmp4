//
// Created by mkh on 19.10.2020.
//

#include "freebox.hh"

Free::Free( std::istream & is, uint32_t sz ) : Atom( is ),m_content_size( sz - 8 ) {
    if( m_content_size && !is_container( is ) ){
        is.seekg( position() + std::streampos(size()) );
    }
}

void Free::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "contents size=" << m_content_size;
}

bool Free::is_container(std::istream &f) {
    size_t initial_pos = f.tellg();
    size_t check_sz = m_content_size > sizeof(uint32_t) ? sizeof(uint32_t) : m_content_size;
    char c;
    f.get( c );

    for( size_t i{1}; i < check_sz; ++i ) {
        char another_c;
        f.get( another_c );
        if( another_c != c ) {
            f.seekg( initial_pos );
            return true;
        }
    }
    return false;
}
