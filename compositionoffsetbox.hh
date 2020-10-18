#ifndef COMPOSITIONOFFSETBOX_HH
#define COMPOSITIONOFFSETBOX_HH

#include <fstream>
#include <vector>

class CompositionOffsetBox
{
public:
    CompositionOffsetBox( std::ifstream& f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::pair< uint32_t /*sample_count*/, uint32_t/*sample_offset*/ > > m_entries;

    friend std::ostream & operator <<( std::ostream& out, const CompositionOffsetBox& ctts );
};

#endif // COMPOSITIONOFFSETBOX_HH
