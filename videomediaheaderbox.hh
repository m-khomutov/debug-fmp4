#ifndef VIDEOMEDIAHEADERBOX_HH
#define VIDEOMEDIAHEADERBOX_HH

#include <fstream>

class VideoMediaHeaderBox
{
public:
    VideoMediaHeaderBox(  std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint16_t m_graphicsmode{0};
    uint16_t m_opcolor[3]{0, 0, 0};

    friend std::ostream & operator <<( std::ostream& out, const VideoMediaHeaderBox& vmhd );
};

#endif // VIDEOMEDIAHEADERBOX_HH
