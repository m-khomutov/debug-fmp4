#ifndef COMPOSITIONOFFSETBOX_HH
#define COMPOSITIONOFFSETBOX_HH

#include "atom.h"
#include <vector>

class CompositionOffsetBox : public Atom {
public:
    CompositionOffsetBox( std::istream& is );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::pair< uint32_t /*sample_count*/, uint32_t/*sample_offset*/ > > m_entries;

private:
    void fout( std::ostream& out ) const override;
};

#endif // COMPOSITIONOFFSETBOX_HH
