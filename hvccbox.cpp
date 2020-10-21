//
// Created by mkh on 19.10.2020.
//

#include "hvccbox.hh"
#include <iomanip>
#include <cstring>

HvcCBox::ConfigSet::ConfigSet( std::istream & is ) {
    m_type = is.get();

    uint16_t count;
    is.read( (char*)&count, sizeof(count) );
    count = be16toh(count);

    for( uint16_t i(0); i < count; ++i ) {
        uint16_t len;
        is.read( (char*)&len, sizeof(len) );
        len = be16toh(len);
        m_data.push_back( std::vector< uint8_t >() );
        f_store_to_rbsp( is, len, &m_data.back() );
    }
}
void HvcCBox::ConfigSet::fout( std::ostream & out, const Atom * atom ) const {
    atom->indent( out );
    out << std::dec << "Config Set " << int(m_type);
    for( size_t i(0); i < m_data.size(); ++i ) {
        out << " [";
        for( auto c : m_data[i] )
            out << std::hex << std::setw(2) << std::setfill('0') << int(c) << " " << std::dec;
        out << "]";
    }
}
void HvcCBox::ConfigSet::f_store_to_rbsp( std::istream& is, uint16_t length, std::vector< uint8_t > * rbsp ) {
    int zeroes = 0;
    for( size_t i(0); i < length; ++i ) {
        uint8_t c = is.get();
        if( c == 0 ) {
            ++zeroes;
        }
        else if( c == 3 && zeroes >= 2 ) {
            zeroes = 0;
            continue;
        }
        else
            zeroes = 0;
        rbsp->push_back( c );
    }
}

HvcCBox::HvcCBox( std::istream& is ) : Atom( is ) {
    is.get();  // 1
    is.read( (char*)m_general_config.data(), kGeneralConfigSize );
    is.read( (char*)&m_min_spacial_segmentation, sizeof(m_min_spacial_segmentation) );
    m_min_spacial_segmentation = be16toh(m_min_spacial_segmentation);
    is.get();  // uint8_t(0xfc) Don't set parallelism type since segmentation isn't set
    is.get();  // uint8_t(0xfc | sps.chroma_format_idc & 3) );
    is.get();  // uint8_t(0xf8 | ((sps.bit_depth_luma-8) & 0x07)) );
    is.get();  // uint8_t(0xf8 | ((sps.bit_depth_chroma-8) & 0x07)) );
    is.read( (char*)&m_framerate, sizeof(m_framerate) );
    m_framerate = be16toh(m_framerate);
    is.get();  // uint8_t(((vps.max_sub_layers & 7) << 3) | (vps.temporal_id_nesting << 2) | 3) );

    uint8_t number_of_nalus = is.get();
    for( uint8_t i(0); i < number_of_nalus; ++i )
        m_config_set_nalus.emplace_back( is );
}

void HvcCBox::fout( std::ostream &out ) const {
    Atom::fout( out );

    out << "config: [ " << std::hex << std::setw(2) << std::setfill( '0' );
    for( auto c : m_general_config )
        out << int(c) << " ";
    out << "] ";
    out << "min spacial segmentation: " << m_min_spacial_segmentation << " framerate: " << m_framerate;

    for( auto set : m_config_set_nalus ) {
        out << "\n";
        set.fout( out, this );
    }
}
