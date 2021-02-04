//
// Created by mkh on 15.10.2019.
//

#include "mediadatabox.hh"

#include <iomanip>

MediaDataBox::MediaDataBox( std::istream& is, const TrunMap &trunMap ) : Atom( is ) {
    if( !trunMap.empty() ) {
        is.seekg( position() + std::streampos(8) );

        TrunMap::const_iterator trun_iter = trunMap.begin();
        if( trun_iter != trunMap.end() ) {
            for( auto sample : *(trun_iter->second) ) {
                f_read_nalunit( is );
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
    else {
        Indicator indicator( is, size() );
        while ( is.tellg() != indicator.end() ) {
            if( is.good() ) {
                f_read_nalunit( is );
                indicator.show( is );
            }
            else
                break;
        }
    }
    if( size() ) {
        is.seekg( position() + std::streampos(size()) );
    }
    else
        is.seekg( 0, std::ios_base::end );
}

void MediaDataBox::f_read_nalunit( std::istream& is ) {
    union {
        uint32_t value;
        char buffer[sizeof(value)];
    } u;
    size_t p = is.tellg();
    is.read( u.buffer, sizeof(u.buffer) );

    uint32_t nalusz = be32toh(u.value);
    char c = is.get();
    is.seekg( p + sizeof(u.buffer) + nalusz );
    if( is.good() ) {
        m_nalu_vector.push_back( Nalu( c&0x1f, nalusz, p ) );
    }
}

void MediaDataBox::fout( std::ostream &out ) const {
    if( !m_nalu_vector.empty() ) {
        out << "\n";
        indent( out );
        out << "video(type:size:offset) : {\n";
        for( auto nalu : m_nalu_vector ) {
            out << std::hex << "{" << int(nalu.type) << std::dec << ":" << nalu.size << ":" << nalu.offset << "}";
        }
        out << "}";
    }
    if( !m_audio_vector.empty() ) {
        out << "\n";
        indent( out );
        out << "audio(size:offset) : {\n";
        for( auto pack : m_audio_vector ) {
            out << " " << pack.size << ":" << pack.offset << "[ ";
            for( uint32_t i(0); i < (pack.size > sizeof(pack.data) ? sizeof(pack.data) : pack.size); ++i )
                out << std::hex << std::setfill( '0' ) << std::setw( 2 ) << int(pack.data[i]) << ' ' << std::dec;
            out << "]\n";
        }
        out << "}";
    }
    Atom::fout( out );
}
