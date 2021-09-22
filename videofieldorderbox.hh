//
// Created by mkh on 12.04.2021.
//
#ifndef DEBUG_FMP4_VIDEOFIELDORDERBOX_H
#define DEBUG_FMP4_VIDEOFIELDORDERBOX_H

#include "atom.h"

class VideoFieldOrderBox : public Atom {
public:
    VideoFieldOrderBox( std::istream & is );

private:
    uint16_t m_order;

private:
    void fout( std::ostream& out ) const override;
};

#endif // DEBUG_FMP4_VIDEOFIELDORDERBOX_H
