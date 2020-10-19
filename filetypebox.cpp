#include "filetypebox.hh"

FileTypeBox::FileTypeBox( std::istream & is, uint32_t sz ) : Atom( is ) {

    union {
        uint32_t value;
        char buffer[4];
    } u;
    is.read( (char*)&u.value, sizeof(u.value) );
    m_majorBrand = std::string(u.buffer, sizeof(u.buffer));
    is.read( (char*)&m_minorVersion, sizeof(m_minorVersion) );

    sz -= 4 * sizeof(uint32_t);
    while( sz ) {
        uint32_t tmp;
        is.read( (char*)&u.value, sizeof(u.value) );
        m_compatibleBrands.push_back( std::string(u.buffer, sizeof(u.buffer)) );
        sz -= sizeof(uint32_t);
    }
}

void FileTypeBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "\nmajor brand='" << m_majorBrand << "' minor version=" << be32toh( m_minorVersion ) << " compatible brands={";
    for( size_t i(0); i < m_compatibleBrands.size(); ++i ) {
        out << m_compatibleBrands[i];
        out << (i != m_compatibleBrands.size()-1 ? " " : "}");
    }
}
