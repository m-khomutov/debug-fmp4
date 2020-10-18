#ifndef TRACKHEADERBOX_HH
#define TRACKHEADERBOX_HH

#include <fstream>

class TrackHeaderBox
{
public:
    TrackHeaderBox( std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint64_t m_creationTime;
    uint64_t m_modificationTime;
    uint32_t m_trackID;
    uint64_t m_duration;

    int16_t m_layer;
    int16_t m_alternateGroup;
    int16_t m_volume;
    int32_t m_matrix[9];

    uint32_t m_width;
    uint32_t m_height;

    friend std::ostream & operator <<( std::ostream& out, const TrackHeaderBox& mvhd );

};

#endif // TRACKHEADERBOX_HH
