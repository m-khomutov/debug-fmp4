#ifndef FILETYPEBOX_HH
#define FILETYPEBOX_HH

#include <fstream>
#include <string>
#include <vector>

class FileTypeBox
{
public:
    FileTypeBox( std::ifstream & f, uint32_t sz );

private:
    std::string m_majorBrand;
    uint32_t m_minorVersion;

    std::vector< std::string > m_compatibleBrands;

    friend std::ostream & operator <<( std::ostream& out, const FileTypeBox& ftyp );
};

#endif // FILETYPEBOX_HH
