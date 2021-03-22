//
// Created by mkh on 15.10.2019.
//

#ifndef FMP4VERIFIER_MEDIADATABOX_HH
#define FMP4VERIFIER_MEDIADATABOX_HH

#include "trackfragmentrunbox.hh"
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <iostream>

class Indicator {
public:
    Indicator( std::istream& is, size_t atom_size )
    : m_begin( is.tellg() ),m_end( m_begin + std::streampos(atom_size) ),m_duration( atom_size ) {
        if( m_duration == 0 ) {
            is.seekg( 0, std::ios_base::end );
            m_end = is.tellg();
            is.seekg( m_begin );
            m_duration = m_end - m_begin;
        }
        ioctl( STDOUT_FILENO, TIOCGWINSZ, &m_winsz );
    }
    std::streampos begin() const {
        return m_begin;
    }
    std::streampos end() const {
        return m_end;
    }
    void show( std::istream& is ) {
        std::streampos cur = is.tellg();
        std::streampos percent = std::streampos(100 * is.tellg()) / m_duration;
        size_t count = m_winsz.ws_col * percent / 100;
        if( count > m_count ) {
            for( size_t i(0); i < count - m_count; ++i )
                std::cerr << "#";
            m_count = count;
        }
    }
private:
    std::streampos m_begin;
    std::streampos m_end;
    std::streampos m_duration;
    winsize m_winsz;
    size_t m_count { 0 };
};

class MediaDataBox : public Atom {
public:
    MediaDataBox( std::istream & is, const TrunMap &trunMap );

private:
    struct Nalu {
        uint8_t type;
        uint32_t size;
        size_t offset;
        Nalu( uint8_t t, uint32_t s, size_t o ) : type( t ),size( s ),offset( o ) {}
    };
    struct AudioPack {
        uint32_t size;
        size_t offset;
        uint8_t data[9];

        AudioPack( uint32_t s, size_t o ) : size( s ),offset( o ) {}
    };
    struct TextPack {
        uint32_t size;
        size_t offset;
        std::string text;
        std::vector< Atom > modifiers;

        TextPack( uint32_t s, size_t o ) : size( s ),offset( o ) {}
    };
    std::vector< Nalu > m_nalu_vector;
    std::vector< AudioPack > m_audio_vector;
    std::vector< TextPack > m_text_vector;

private:
  void f_read_video( std::istream& is, TrackFragmentRunBox* trun );
  void f_read_audio( std::istream& is, TrackFragmentRunBox* trun );
  void f_read_text( std::istream& is, TrackFragmentRunBox* trun );
  void f_read_nalunit( std::istream& is );
  void fout( std::ostream& out ) const override;
};


#endif //FMP4VERIFIER_MEDIADATABOX_HH