#ifndef AVCCBOX_HH
#define AVCCBOX_HH

#include <fstream>
#include <vector>

class AvcCBox
{
public:
    AvcCBox( std::ifstream& f, uint32_t sz );

private:
    uint8_t m_version;       // always 0x01
    uint8_t m_profile;       // sps[0][1]
    uint8_t m_compatibility; // sps[0][2]
    uint8_t m_level;         // sps[0][3]
    uint8_t m_nalulen;

    std::vector< std::vector< uint8_t > > m_sps;
    std::vector< std::vector< uint8_t > > m_pps;

    friend std::ostream & operator <<( std::ostream& out, const AvcCBox& stsd );
};

#endif // AVCCBOX_HH
