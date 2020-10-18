#ifndef MOVIEEXTENDSHEADERBOX_HH
#define MOVIEEXTENDSHEADERBOX_HH

#include <fstream>

class MovieExtendsHeaderBox
{
public:
    MovieExtendsHeaderBox( std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint64_t m_fragmentDuration;

    friend std::ostream & operator <<( std::ostream& out, const MovieExtendsHeaderBox& mvhd );
};

#endif // MOVIEEXTENDSHEADERBOX_HH
