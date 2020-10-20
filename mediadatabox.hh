//
// Created by mkh on 15.10.2019.
//

#ifndef FMP4VERIFIER_MEDIADATABOX_HH
#define FMP4VERIFIER_MEDIADATABOX_HH

#include "trackfragmentrunbox.hh"

#include "vector"

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
    std::vector< Nalu > m_nalu_vector;
    std::vector< AudioPack > m_audio_vector;

private:
    void fout( std::ostream& out ) const override;
};


#endif //FMP4VERIFIER_MEDIADATABOX_HH