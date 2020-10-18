#ifndef SOUNDMEDIAHEADERBOX_HH
#define SOUNDMEDIAHEADERBOX_HH

#include <fstream>

class SoundMediaHeaderBox
{
public:
    SoundMediaHeaderBox( std::ifstream& f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint16_t m_balance{0};

    friend std::ostream & operator <<( std::ostream& out, const SoundMediaHeaderBox& vmhd );
};

#endif // SOUNDMEDIAHEADERBOX_HH
