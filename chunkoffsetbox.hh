//
// Created by mkh on 09.02.2021.
//

#ifndef DEBUG_FMP4_CHUNKOFFSETBOX_H
#define DEBUG_FMP4_CHUNKOFFSETBOX_H

#include "atom.h"
#include <vector>

template< typename OffsetClass >
class ChunkOffsetBox : public Atom {
public:
    ChunkOffsetBox( std::istream& is );
    ~ChunkOffsetBox() = default;

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< OffsetClass > m_entries;

private:
    void fout( std::ostream& out ) const override;
};


class chunkoffsetbox {

};



#endif //DEBUG_FMP4_CHUNKOFFSETBOX_H
