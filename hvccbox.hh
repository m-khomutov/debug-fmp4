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
    std::vector< uint8_t > m_data;

private:
    void fout( std::ostream& out ) const override;
};


#endif //DEBUG_MP4_HVCCBOX_HH
