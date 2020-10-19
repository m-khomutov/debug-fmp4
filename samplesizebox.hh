#ifndef SAMPLESIZEBOX_HH
#define SAMPLESIZEBOX_HH

#include "atom.h"
#include <vector>

class SampleSizeBox : public Atom {
public:
    SampleSizeBox( std::istream & is );

private:
    uint8_t m_version;
    uint32_t m_flags;

    uint32_t m_samplesize;
    uint32_t m_samplecount;
    std::vector< uint32_t > m_entrysizeVector;

private:
    void fout( std::ostream& out ) const override;
};

#endif // SAMPLESIZEBOX_HH
