#ifndef MOVIEHEADERBOX_HH
#define MOVIEHEADERBOX_HH

#include "atom.h"

class MovieHeaderBox : public Atom {
public:
    MovieHeaderBox( std::istream & f, uint32_t sz );

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

private:
    void fout( std::ostream& out ) const override;
};

#endif // MOVIEHEADERBOX_HH
