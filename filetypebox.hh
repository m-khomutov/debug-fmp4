#ifndef FILETYPEBOX_HH
#define FILETYPEBOX_HH

#include "atom.h"

#include <fstream>
#include <string>
#include <vector>

class FileTypeBox : public Atom
{
public:
    FileTypeBox( std::istream & f, uint32_t sz );

private:
    std::string m_majorBrand;
    uint32_t m_minorVersion;

    std::vector< std::string > m_compatibleBrands;

private:
    void fout( std::ostream& out ) const override;
};

#endif // FILETYPEBOX_HH
