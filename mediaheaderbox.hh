#ifndef MEDIAHEADERBOX_HH
#define MEDIAHEADERBOX_HH

#include <fstream>

class MediaHeaderBox
{
public:
    MediaHeaderBox( std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint64_t m_creationTime;
    uint64_t m_modificationTime;
    uint32_t m_timescale;
    uint64_t m_duration;

    char m_language[ 3 ];

    friend std::ostream & operator <<( std::ostream& out, const MediaHeaderBox& mdhd );
};

#endif // MEDIAHEADERBOX_HH
