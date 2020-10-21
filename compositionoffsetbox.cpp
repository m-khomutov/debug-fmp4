#include "compositionoffsetbox.hh"

CompositionOffsetBox::CompositionOffsetBox( std::istream& is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    uint32_t count{0};
    is.read( (char*)&count, sizeof(count) );
    count = be32toh( count );
    for( uint32_t i(0); i < count; ++i ) {
        m_entries.push_back( std::make_pair(0, 0) );

        is.read( (char*)&m_entries.back().first, sizeof(uint32_t) );
        m_entries.back().first = be32toh( m_entries.back().first );

        is.read( (char*)&m_entries.back().second, sizeof(uint32_t) );
        m_entries.back().second = be32toh( m_entries.back().second );
    }
}

void CompositionOffsetBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " number of offsets: " << m_entries.size();
    out << " samples(count:offset)={";
    for( auto p : m_entries )
        out << p.first << ":" << p.second << " ";
    out << "}";
}