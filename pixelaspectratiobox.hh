//
// Created by mkh on 12.04.2021.
//
#ifndef DEBUG_FMP4_PIXELASPECTRATIOBOX_HH
#define DEBUG_FMP4_PIXELASPECTRATIOBOX_HH

#include "atom.h"

class PixelAspectRatioBox : public Atom {
public:
    PixelAspectRatioBox( std::istream & is );

private:
    uint32_t m_hSpacing;
    uint32_t m_vSpacing;

private:
    void fout( std::ostream& out ) const override;
};

#endif // DEBUG_FMP4_PIXELASPECTRATIOBOX_HH
