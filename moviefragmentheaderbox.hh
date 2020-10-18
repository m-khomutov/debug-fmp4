#ifndef MOVIEFRAGMENTHEADERBOX_HH
#define MOVIEFRAGMENTHEADERBOX_HH

#include <fstream>

class MovieFragmentHeaderBox
{
public:
    MovieFragmentHeaderBox( std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint32_t m_sequenceNumber;

    friend std::ostream & operator <<( std::ostream& out, const MovieFragmentHeaderBox& mvhd );
};

#endif // MOVIEFRAGMENTHEADERBOX_HH
