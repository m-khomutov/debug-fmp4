#ifndef TIMETOSAMPLEBOX_HH
#define TIMETOSAMPLEBOX_HH

#include "atom.h"
#include <vector>

class TimeToSampleBox : public Atom {
public:
    TimeToSampleBox( std::istream& f );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::pair< uint32_t /*sample_count*/, uint32_t/*sample_delta*/ > > m_entries;

private:
    void fout( std::ostream& out ) const override;
};

#endif // TIMETOSAMPLEBOX_HH
