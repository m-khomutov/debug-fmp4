//
// Created by mkh on 19.10.2020.
//

#ifndef DEBUG_MP4_HVCCBOX_HH
#define DEBUG_MP4_HVCCBOX_HH

#include "atom.h"
#include <vector>

class HvcCBox : public Atom {
public:
    HvcCBox( std::istream& f );

private:
    class ConfigSet {
    public:
        ConfigSet() = default;
        ConfigSet( std::istream & is );

        void fout( std::ostream & out, const Atom * atom ) const;

    private:
        uint8_t m_type;
        std::vector< std::vector< uint8_t > > m_data;

    private:
        void f_store_to_rbsp( std::istream& is, uint16_t length, std::vector< uint8_t > * rbsp );
    };

    static const uint32_t kGeneralConfigSize = 12;

    std::array< uint8_t, kGeneralConfigSize > m_general_config;
    uint16_t m_min_spacial_segmentation;
    uint16_t m_framerate;
    std::vector< ConfigSet > m_config_set_nalus;

private:
    void fout( std::ostream& out ) const override;
};


#endif //DEBUG_MP4_HVCCBOX_HH
