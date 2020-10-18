#ifndef TIMETOSAMPLEBOX_HH
#define TIMETOSAMPLEBOX_HH

#include <fstream>
#include <vector>

class TimeToSampleBox
{
public:
    TimeToSampleBox( std::ifstream& f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::pair< uint32_t /*sample_count*/, uint32_t/*sample_delta*/ > > m_entries;

    friend std::ostream & operator <<( std::ostream& out, const TimeToSampleBox& stts );
};

#endif // TIMETOSAMPLEBOX_HH
