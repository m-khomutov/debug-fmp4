#ifndef VIDEOMEDIAHEADERBOX_HH
#define VIDEOMEDIAHEADERBOX_HH

#include "atom.h"

class VideoMediaHeaderBox : public Atom {
public:
    VideoMediaHeaderBox(  std::istream & is, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint16_t m_graphicsmode{0};
    uint16_t m_opcolor[3]{0, 0, 0};

private:
    void fout( std::ostream& out ) const override;
};

#endif // VIDEOMEDIAHEADERBOX_HH
