//
// Created by mkh on 27.01.2021.
//

#ifndef DEBUG_FMP4_INITIALOBJECTDESCRIPTORBOX_H
#define DEBUG_FMP4_INITIALOBJECTDESCRIPTORBOX_H

#include "atom.h"
#include <vector>

class InitialObjectDescriptorBox : public Atom {
public:
    InitialObjectDescriptorBox( std::istream & f, uint32_t sz );

private:
    std::vector< uint8_t > m_data;

private:
    void fout( std::ostream& out ) const override;
};

#endif // DEBUG_FMP4_INITIALOBJECTDESCRIPTORBOX_H
