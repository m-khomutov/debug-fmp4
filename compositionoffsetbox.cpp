#include "compositionoffsetbox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const CompositionOffsetBox& stts ) {

    out << "version=" << int(stts.m_version) << " flags=" << std::hex << stts.m_flags << std::dec;
    out << " number of offsets: " << stts.m_entries.size();
/*    out << " samples={";
    for( auto p : stts.m_entries )
        out << "'count=" << p.first << " offset=" << p.second << "' ";
    out << "}";*/

    return out;
}

CompositionOffsetBox::CompositionOffsetBox( std::ifstream& f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    uint32_t count{0};
    f.read( (char*)&count, sizeof(count) );
    count = be32toh( count );
    for( uint32_t i(0); i < count; ++i ) {
        m_entries.push_back( std::make_pair(0, 0) );

        f.read( (char*)&m_entries.back().first, sizeof(uint32_t) );
        m_entries.back().first = be32toh( m_entries.back().first );

        f.read( (char*)&m_entries.back().second, sizeof(uint32_t) );
        m_entries.back().second = be32toh( m_entries.back().second );
    }

}
