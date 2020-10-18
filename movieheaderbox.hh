#ifndef MOVIEHEADERBOX_HH
#define MOVIEHEADERBOX_HH

#include <fstream>

class MovieHeaderBox
{
public:
    MovieHeaderBox( std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint64_t m_creationTime;
    uint64_t m_modificationTime;
    uint32_t m_timescale;
    uint64_t m_duration;

    int32_t m_rate;
    int16_t m_volume;
    int32_t m_matrix[9];

    uint32_t m_nextTrackID;

    friend std::ostream & operator <<( std::ostream& out, const MovieHeaderBox& mvhd );
};

#endif // MOVIEHEADERBOX_HH
