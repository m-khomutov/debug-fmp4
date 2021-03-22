//
// Created by mkh on 17.03.2021.
//
#ifndef DEBUG_FMP4_NULLMEDIAHEADERBOX_H
#define DEBUG_FMP4_NULLMEDIAHEADERBOX_H

#include "atom.h"

class NullMediaHeaderBox : public Atom {
public:
    NullMediaHeaderBox( std::istream& f );

private:
    uint8_t m_version;
    uint32_t m_flags;

private:
  void fout( std::ostream& out ) const override;
};

#endif // DEBUG_FMP4_NULLMEDIAHEADERBOX_H
