#ifndef AVCCBOX_HH
#define AVCCBOX_HH

#include "atom.h"
#include <vector>

class AvcCBox : public Atom {
public:
    AvcCBox( std::istream& f );

private:
    uint8_t m_version;       // always 0x01
    uint8_t m_profile;       // sps[0][1]
    uint8_t m_compatibility; // sps[0][2]
    uint8_t m_level;         // sps[0][3]
    uint8_t m_nalulen;

    std::vector< std::vector< uint8_t > > m_sps;
    std::vector< std::vector< uint8_t > > m_pps;

private:
    void fout( std::ostream& out ) const override;
};

#endif // AVCCBOX_HH
