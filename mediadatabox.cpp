//
// Created by mkh on 15.10.2019.
//

#include "mediadatabox.hh"

#include <iomanip>

MediaDataBox::MediaDataBox( std::istream& is, const TrunMap &trunMap ) : Atom( is ) {
    if( !trunMap.empty() ) {
        is.seekg( position() + std::streampos(8) );

        union {
            uint32_t value;
            char buffer[sizeof(value)];
        } u;

        TrunMap::const_iterator trun_iter = trunMap.begin();
        if( trun_iter != trunMap.end() ) {
            for( auto sample : *(trun_iter->second) ) {
                size_t p = is.tellg();
                is.read( u.buffer, sizeof(u.buffer) );

                uint32_t nalusz = be32toh(u.value);
                char c = is.get();
                m_nalu_vector.push_back( Nalu( (c>>1)&0x3f, nalusz, p ) );
                is.seekg( p + sizeof(u.buffer) + nalusz );
            }
            std::advance( trun_iter, 1 );
            if( trun_iter != trunMap.end() ) {
                for( auto sample : *(trun_iter->second) ) {
                    size_t p = is.tellg();
                    m_audio_vector.push_back( AudioPack( sample.size(), p ) );
                    for( uint32_t i(0); i < (sample.size() > sizeof(AudioPack::data) ? sizeof(AudioPack::data) : sample.size()); ++i )
                        m_audio_vector.back().data[i] = is.get();
                    is.seekg( p + sample.size() );
                }
            }
        }
    }
    is.seekg( position() + std::streampos(size()) );
}

void MediaDataBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    if( !m_nalu_vector.empty() ) {
        indent( out );
        out << "\nvideo(type:size:offset) : {\n";
        for( auto nalu : m_nalu_vector )
            out << std::hex << " 0x" << int(nalu.type) << std::dec << ":" << nalu.size << ":" << nalu.offset << "\n";
        out << "}";
    }
    if( !m_audio_vector.empty() ) {
        indent( out );
        out << "\naudio(size:offset) : {\n";
        for( auto pack : m_audio_vector ) {
            out << " " << pack.size << ":" << pack.offset << "[ ";
            for( uint32_t i(0); i < (pack.size > sizeof(pack.data) ? sizeof(pack.data) : pack.size); ++i )
                out << std::hex << std::setfill( '0' ) << std::setw( 2 ) << int(pack.data[i]) << ' ' << std::dec;
            out << "]\n";
        }
        out << "}";
    }
}
