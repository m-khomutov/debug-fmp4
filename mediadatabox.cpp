//
// Created by mkh on 15.10.2019.
//

#include "mediadatabox.hh"

#include <iostream>
#include <iomanip>

std::ostream & operator <<( std::ostream& out, const MediaDataBox& mdat ) {

    out << "vdata(type:size:offset) : {\n";
    for( auto nalu : mdat.m_nalu_vector )
        out << int(nalu.type) << ":" << nalu.size << ":" << nalu.offset << " ";
    out << "\n}\nadata(size:offset:start) : {\n";
    for( auto pack : mdat.m_pack_vector ) {
        out << pack.size << ":" << pack.offset << "[ ";
	for( uint32_t i(0); i < (pack.size > sizeof(pack.data) ? sizeof(pack.data) : pack.size); ++i )
            out << std::hex << std::setfill( '0' ) << std::setw( 2 ) << int(pack.data[i]) << ' ' << std::dec;	
        out << "]\n";
    }
    out << "}";

    return out;
}

MediaDataBox::MediaDataBox( std::ifstream &f, size_t moof_position, const TrunMap & trun_map, uint32_t sz ) : m_datasz( sz - 8 ) {
    size_t pos = f.tellg();

/*    std::vector< uint8_t > v(m_dataSize);
    f.read((char*)v.data(), v.size());
    std::ofstream of("qqq.mp4a");
    of.write((char*)v.data(), v.size());
    return;*/

    union {
	uint32_t value;
        char buffer[sizeof(value)];
    } u;

    uint32_t off = 0;
    TrunMap::const_iterator trun_iter = trun_map.begin();
    if( trun_iter != trun_map.end() ) {
        for( auto sample : *(trun_iter->second) ) {
            size_t p = f.tellg();
	    f.read( u.buffer, sizeof(u.buffer) );
            
	    uint32_t nalusz = be32toh(u.value);
	    char c = f.get();
	    m_nalu_vector.push_back( Nalu( (c>>1)&0x3f, nalusz, p ) );
	    fprintf( stderr, "SIZE: %d - %d\n", sample.size(), nalusz );
	    off += sizeof(u.buffer) + nalusz;
            f.seekg( pos + off );
        }
        std::advance( trun_iter, 1 );
        if( trun_iter != trun_map.end() ) {
            for( auto sample : *(trun_iter->second) ) {
	        m_pack_vector.push_back( AudioPack( sample.size(), f.tellg() ) );
	        for( uint32_t i(0); i < (sample.size() > sizeof(AudioPack::data) ? sizeof(AudioPack::data) : sample.size()); ++i )
                    m_pack_vector.back().data[i] = f.get();
	        off += sample.size();
                f.seekg( pos + off );
	    }
        }
    }
    std::cerr << "OFF: " << off << "DATASZ: " << m_datasz << std::endl;
    f.seekg( pos + m_datasz );

/*    uint32_t off = 0;
    while( off < m_dataSize ) {
        size_t p = f.tellg(); 
	if( p < moof_position ) {//+ trunoffset - 8 ) {
            f.read( u.buffer, sizeof(u.buffer) );

            uint32_t nalusize = be32toh(u.value);
	    char c = f.get();
	    m_nalu_vector.push_back( Nalu( (c>>1)&0x3f, nalusize, p ) );
	    off += sizeof(u.buffer) + nalusize;
	}
	else {
            fprintf(stderr, "audio pos = %d\n", f.tellg());
	    for( int i(0); i < 18; ++i)fprintf(stderr,"%02x ", f.get());fprintf(stderr,"\n");
	    off = m_dataSize;
	}
        f.seekg( pos + off );
    }*/
/*
    if( tmp[0] == 0 && tmp[1] == 0 ) {
        if( tmp[2] == 1 || (tmp[2] == 0 && tmp[3] == 1) ) {
            f_read_frames( f );
            return;
        }
    }

    uint32_t datasz{0};
    while( datasz < m_dataSize ) {
        uint32_t frame_size;
        f.read( (char*)&frame_size, sizeof(frame_size) );
        frame_size = be32toh( frame_size );
        f.read( (char*)m_data.data(), m_data.size() );
        for( auto c : m_data )
            std::cerr << std::hex << std::setfill( '0' ) << std::setw( 2 ) << int(c) << ' ' << std::dec;
        std::cerr << " of SIZE: " << frame_size << " (" <<std::hex << frame_size << ")" << std::dec << std::endl;
        //f.seekg( frame_size-m_data.size(), std::ios_base::cur );
        datasz += sizeof(frame_size)+frame_size;
        f.seekg( pos+datasz );
        std::cerr << sz << " -- " << datasz << "." << frame_size << std::endl;
    }*/
    //f.seekg( pos + (sz - 8) );
}

void MediaDataBox::f_read_frames( std::ifstream &f ) {
    std::vector< uint8_t > data(m_datasz);
    f.read( (char*)data.data(), data.size() );

    uint8_t *nalu_ptr{nullptr};
    for( int i(0); i < m_datasz; ++i ) {      // ищем 0 0 {0,1,7,8,5}
        if( data[i] == 0 && data[i+1] == 0 ) {  // нашли 0 0
            if( data[i+2] == 0 ) {              // нашли 0 0 0
                if( data[i+3] == 1 ) {          // нашли 0 0 0 1
                    if( nalu_ptr ) {
                        long sz = data.data()+i - nalu_ptr;
                        f_show_frame( nalu_ptr, sz );
                    }
                    nalu_ptr = data.data()+i+4;
                    i += 2;
                }
            }
            else if( data[i+2] == 1 ) {         // нашли 0 0 1
                if( nalu_ptr ) {
                    long sz = data.data()+i - nalu_ptr;
                    f_show_frame( nalu_ptr, sz );
                }
                nalu_ptr = data.data()+i+3;
                i += 2;
            }
        }
    }
    if( nalu_ptr )
        f_show_frame( nalu_ptr, m_datasz - (nalu_ptr-data.data()) );
}

void MediaDataBox::f_show_frame( uint8_t *start, size_t size ) {
    for( size_t i(0); i < (size > 20 ? 20 : size);  ++i )
        std::cerr << std::hex << std::setfill( '0' ) << std::setw( 2 ) << int(start[i]) << ' ' << std::dec;
    std::cerr << " of SIZE: " << size << std::endl;
}
