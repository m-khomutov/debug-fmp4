//
// Created by mkh on 15.10.2019.
//

#include "mediadatabox.hh"

#include <iomanip>

MediaDataBox::MediaDataBox( std::istream& is, const TrunMap &trunMap ) : Atom( is ) {
    if( !trunMap.empty() ) {
        is.seekg( position() + std::streampos(8) );

        for( auto tr : trunMap ) {
            switch( tr.first ) {
            case Atom::vide:
                f_read_video( is, tr.second.get() );
                break;
            case Atom::soun:
                f_read_audio( is, tr.second.get() );
                break;
            case Atom::text:
                f_read_text( is, tr.second.get() );
                break;
            }
        }
    }
    if( size() ) {
        is.seekg( position() + std::streampos(size()) );
    }
    else
        is.seekg( 0, std::ios_base::end );
}

void MediaDataBox::f_read_video( std::istream &is, TrackFragmentRunBox* trun ) {
    for( auto sample : *trun ) {
        f_read_nalunit( is, sample );
    }
}

void MediaDataBox::f_read_audio( std::istream &is, TrackFragmentRunBox* trun ) {
    for( auto sample : *trun ) {
        size_t p = is.tellg();
        m_audio_vector.push_back( AudioPack( sample.size(), p ) );
        for( uint32_t i(0); i < (sample.size() > sizeof(AudioPack::data) ? sizeof(AudioPack::data) : sample.size()); ++i )
            m_audio_vector.back().data[i] = is.get();
        is.seekg( p + sample.size() );
    }
}

void MediaDataBox::f_read_text( std::istream &is, TrackFragmentRunBox* trun ) {
    for( auto sample : *trun ) {
        size_t p = is.tellg();
        m_text_vector.push_back( TextPack( sample.size(), p ) );
        uint16_t textsz;
        is.read( (char*)&textsz, sizeof(textsz) );
        textsz = be16toh( textsz );
        m_text_vector.back().text.resize( textsz );
        is.read( &m_text_vector.back().text[0], textsz );
        int off = sample.size() - (textsz + 2);
        while( off > 0 ) {
            try {
                m_text_vector.back().modifiers.push_back( Atom( is ) );
                off -= m_text_vector.back().modifiers.back().size();
            }
            catch( const std::logic_error& err ) {
                std::cerr << "text modifier error : " << err.what();
                break;
            }
        }
        is.seekg( p + sample.size() );
    }
}

void MediaDataBox::f_read_nalunit( std::istream& is, const TrackFragmentRunBox::Sample& sample ) {
    union {
        uint32_t value;
        char buffer[sizeof(value)];
    } u;
    size_t off = 0;
    while( off < sample.size() ) {
        size_t p = is.tellg();
        is.read( u.buffer, sizeof(u.buffer) );

        uint32_t nalusz = be32toh(u.value);
        char c = is.get();
        is.seekg( p + sizeof(u.buffer) + nalusz );
        if( is.good() ) {
            m_nalu_vector.push_back( Nalu( c&0x1f, nalusz, p ) );
        }
        off += sizeof(u.buffer) + nalusz;
    }
}

void MediaDataBox::fout( std::ostream &out ) const {
    Atom::fout( out );
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
    if( !m_text_vector.empty() ) {
        out << "\n";
        indent( out );
        out << "text(size:offset:text[ modifiers ]) : {\n";
        for( auto pack : m_text_vector ) {
            out << " " << pack.size << ":" << pack.offset << ":" << pack.text << "[ " << std::hex;
            for( auto m : pack.modifiers )
                out << m << ";";
            out << std::dec << " ]\n";
        }
        out << "}";
    }
}
