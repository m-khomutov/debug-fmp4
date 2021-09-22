//
// Created by mkh on 09.02.2021.
//

#include "chunkoffsetbox.hh"
#include <iostream>

template< typename OffsetClass >
ChunkOffsetBox< OffsetClass >::ChunkOffsetBox( std::istream& is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    uint32_t count{0};
    is.read( (char*)&count, sizeof(count) );
    count = be32toh( count );
    for( uint32_t i(0); i < count; ++i ) {
        OffsetClass offset;
        is.read( (char*)&offset, sizeof(offset) );
        if( sizeof(offset) == sizeof(uint32_t) ) {
            m_entries.push_back( be32toh(offset) );
        }
        else if( sizeof(offset) == sizeof(uint64_t) ) {
            m_entries.push_back( be64toh(offset) );
        }
    }
}

template< typename OffsetClass >
void ChunkOffsetBox< OffsetClass >::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " number of offsets: " << m_entries.size();
    out << " ={ ";
    for( auto p : m_entries )
        out << p << " ";
    out << "}";
}

template class ChunkOffsetBox< uint32_t >;
template class ChunkOffsetBox< uint64_t >;

