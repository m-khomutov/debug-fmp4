#include "handlerreferencebox.hh"

HandlerReferenceBox::HandlerReferenceBox( std::istream & is, std::string * type ) : Atom( is ) {
    union {
        uint32_t val;
        char buf[ 4 ];
    } tmp;

    is.read( (char*)&tmp.val, sizeof(tmp.val) );
    m_version = tmp.val >> 24;
    m_flags = tmp.val & 0xffffff;

    is.read( (char*)&tmp.val, sizeof(tmp.val) );  // pre_defined = 0
    is.read( (char*)&tmp.val, sizeof(tmp.val) );
    m_type = Atom::Value(tmp.val);
    m_typeAsString = std::string( tmp.buf, sizeof(tmp.buf) );
    *type = m_typeAsString;

    for( int i(0); i < 3; ++i )
        is.read( (char*)&tmp.val, sizeof(tmp.val) ); // unsigned int(32)[3] reserved = 0

    for( uint32_t i(0); i < (size() - 8*sizeof(uint32_t)); ++i )
        m_name += is.get();
}

void HandlerReferenceBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "handler='" << m_typeAsString << "' name='" << m_name << "'";
}