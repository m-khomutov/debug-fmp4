//
// Created by mkh on 19.10.2020.
//

#ifndef DEBUG_MP4_FREEBOX_HH
#define DEBUG_MP4_FREEBOX_HH

#include "atom.h"

class Free : public Atom {
public:
    Free( std::istream & f, uint32_t sz );

private:
    uint32_t m_content_size;

private:
    void fout( std::ostream& out ) const override;
};


#endif //DEBUG_MP4_FREEBOX_HH
