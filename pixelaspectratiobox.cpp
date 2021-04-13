//
// Created by mkh on 12.04.2021.
//

#include "pixelaspectratiobox.hh"

namespace {
std::string spacing2str( uint32_t hSpacing, uint32_t vSpacing) {
    if( hSpacing == 1 && vSpacing == 1 )
        return "4:3 square pixels (composite NTSC or PAL)";
    if( hSpacing == 10  && vSpacing == 11 )
        return "4:3 non-square 525 (NTSC)";
    if( hSpacing == 59  && vSpacing == 54 )
        return "4:3 non-square 625 (PAL)";
    if( hSpacing == 4  && vSpacing == 3 )
        return "16:9 analog (composite NTSC or PAL)";
    if( hSpacing == 40  && vSpacing == 33 )
        return "16:9 digital 525 (NTSC)";
    if( hSpacing == 118  && vSpacing == 81 )
        return "16:9 digital 625 (PAL)";
    if( hSpacing == 113  && vSpacing == 118 )
        return "1920x1035 HDTV (per SMPTE 260M-1992)";
    if( hSpacing == 1018  && vSpacing == 1062 )
        return "1920x1035 HDTV (per SMPTE RP 187-1995)";
    return "";
}

}

PixelAspectRatioBox::PixelAspectRatioBox(std::istream &is) : Atom( is ) {
    is.read( (char*)&m_hSpacing, sizeof(m_hSpacing) );
    m_hSpacing = be32toh( m_hSpacing );
    is.read( (char*)&m_vSpacing, sizeof(m_vSpacing) );
    m_vSpacing = be32toh( m_vSpacing );
}

void PixelAspectRatioBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "hSpacing=" << m_hSpacing << " vSpacing=" << m_vSpacing << " " << spacing2str( m_hSpacing, m_vSpacing);
}
