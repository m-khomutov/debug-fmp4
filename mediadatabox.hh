//
// Created by mkh on 15.10.2019.
//

#ifndef FMP4VERIFIER_MEDIADATABOX_HH
#define FMP4VERIFIER_MEDIADATABOX_HH

#include "trackfragmentrunbox.hh"

#include "vector"
#include <fstream>

class MediaDataBox {
public:
    MediaDataBox( std::ifstream & f, size_t moof_position, const TrunMap & trun_map, uint32_t sz );

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
    uint32_t m_datasz;
    std::vector< Nalu > m_nalu_vector;
    std::vector< AudioPack > m_pack_vector;

private:
    void f_read_frames( std::ifstream &f );
    void f_show_frame( uint8_t *start, size_t size );

    friend std::ostream & operator <<( std::ostream& out, const MediaDataBox& mdat );
};


#endif //FMP4VERIFIER_MEDIADATABOX_HH
