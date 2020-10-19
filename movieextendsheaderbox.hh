#ifndef MOVIEEXTENDSHEADERBOX_HH
#define MOVIEEXTENDSHEADERBOX_HH

#include "atom.h"

class MovieExtendsHeaderBox : public Atom {
public:
    MovieExtendsHeaderBox( std::istream & is );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint64_t m_fragmentDuration;

private:
    void fout( std::ostream& out ) const override;
};

#endif // MOVIEEXTENDSHEADERBOX_HH
