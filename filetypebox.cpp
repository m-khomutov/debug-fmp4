#include "filetypebox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const FileTypeBox& ftyp ) {
    out << "major brand='" << ftyp.m_majorBrand << "' minor version=" << be32toh( ftyp.m_minorVersion ) << " compatible brands={";
    for( size_t i(0); i < ftyp.m_compatibleBrands.size(); ++i ) {
        out << ftyp.m_compatibleBrands[i];
        out << (i != ftyp.m_compatibleBrands.size()-1 ? " " : "}");
    }
    return out;
}

FileTypeBox::FileTypeBox( std::ifstream & f, uint32_t sz )
{
    union {
        uint32_t value;
        char buffer[4];
    } u;
    f.read( (char*)&u.value, sizeof(u.value) );
    m_majorBrand = std::string(u.buffer, sizeof(u.buffer));
    f.read( (char*)&m_minorVersion, sizeof(m_minorVersion) );


    sz -= 4 * sizeof(uint32_t);
    while( sz ) {
        uint32_t tmp;
        f.read( (char*)&u.value, sizeof(u.value) );
        m_compatibleBrands.push_back( std::string(u.buffer, sizeof(u.buffer)) );
        sz -= sizeof(uint32_t);
    }
}
