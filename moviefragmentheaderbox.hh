#ifndef MOVIEFRAGMENTHEADERBOX_HH
#define MOVIEFRAGMENTHEADERBOX_HH

#include "atom.h"

class MovieFragmentHeaderBox : public Atom {
public:
    MovieFragmentHeaderBox( std::istream & f );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint32_t m_sequenceNumber;

private:
    void fout( std::ostream& out ) const override;
};

#endif // MOVIEFRAGMENTHEADERBOX_HH
