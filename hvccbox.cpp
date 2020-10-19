//
// Created by mkh on 19.10.2020.
//

#include "hvccbox.hh"

HvcCBox::HvcCBox( std::istream& is ) : Atom( is ) {
    m_data.resize( size()-8 );
    is.read( (char*)m_data.data(), m_data.size() );
}

void HvcCBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << std::endl;
    for( auto c : m_data ) fprintf( stdout, "%02x ", c );
    fprintf( stdout, " : payload size = %d\n", m_data.size() );
}
