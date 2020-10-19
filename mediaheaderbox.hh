#ifndef MEDIAHEADERBOX_HH
#define MEDIAHEADERBOX_HH

#include "atom.h"

class MediaHeaderBox : public Atom {
public:
    MediaHeaderBox( std::istream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint64_t m_creationTime;
    uint64_t m_modificationTime;
    uint32_t m_timescale;
    uint64_t m_duration;

    char m_language[ 3 ];

private:
    void fout( std::ostream& out ) const override;
};

#endif // MEDIAHEADERBOX_HH
