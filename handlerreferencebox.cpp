#include "handlerreferencebox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const HandlerReferenceBox& hdlr ) {
    out << "handler='" << hdlr.m_handlerType << "' name='" << hdlr.m_name << "'";

    return out;
}

HandlerReferenceBox::HandlerReferenceBox( std::ifstream & f, uint32_t sz )
{
    union {
        uint32_t val;
        char buf[ 4 ];
    } tmp;

    f.read( (char*)&tmp.val, sizeof(tmp.val) );
    m_version = tmp.val >> 24;
    m_flags = tmp.val & 0xffffff;

    f.read( (char*)&tmp.val, sizeof(tmp.val) );  // pre_defined = 0
    f.read( (char*)&tmp.val, sizeof(tmp.val) );
    m_handlerType = std::string( tmp.buf, sizeof(tmp.buf) );

    for( int i(0); i < 3; ++i )
        f.read( (char*)&tmp.val, sizeof(tmp.val) ); // unsigned int(32)[3] reserved = 0

    for( uint32_t i(0); i < (sz - 8*sizeof(uint32_t)); ++i )
        m_name += f.get();
}
