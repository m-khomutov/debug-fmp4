//
// Created by mkh on 12.04.2021.
//

#include "videofieldorderbox.hh"

VideoFieldOrderBox::VideoFieldOrderBox(std::istream &is) : Atom( is ) {
    is.read( (char*)&m_order, sizeof(m_order) );
    m_order = be32toh( m_order );
}

void VideoFieldOrderBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "video field order:";
    if( m_order == 1 )
        out << "progressive";
    else if( m_order == 2 )
        out << "2:1 Interlaced";
    else
        out << m_order;
}
