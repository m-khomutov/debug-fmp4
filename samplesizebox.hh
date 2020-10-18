#ifndef SAMPLESIZEBOX_HH
#define SAMPLESIZEBOX_HH

#include <fstream>
#include <vector>

class SampleSizeBox
{
public:
    SampleSizeBox( std::ifstream & f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint32_t m_samplesize;
    uint32_t m_samplecount;
    std::vector< uint32_t > m_entrysizeVector;

    friend std::ostream & operator <<( std::ostream& out, const SampleSizeBox& mvhd );
};

#endif // SAMPLESIZEBOX_HH
