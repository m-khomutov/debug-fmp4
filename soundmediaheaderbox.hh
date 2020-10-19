#ifndef SOUNDMEDIAHEADERBOX_HH
#define SOUNDMEDIAHEADERBOX_HH

#include "atom.h"

class SoundMediaHeaderBox : public Atom {
public:
    SoundMediaHeaderBox( std::istream& f );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint16_t m_balance{0};

private:
    void fout( std::ostream& out ) const override;
};

#endif // SOUNDMEDIAHEADERBOX_HH
