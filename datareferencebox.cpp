#include "datareferencebox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const DataEntryUrlBox& url ) {
    out << "version=" << int(url.m_version) << " flags=" << std::hex << url.m_flags << std::dec;
    out << " location='" << url.m_location << "'";
    return out;
}

DataEntryUrlBox::DataEntryUrlBox( std::ifstream& f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    if( m_flags != 0x000001 ) {
        for( uint32_t i(0); i < (sz - 3*sizeof(uint32_t)); ++i )
        m_location += f.get();
    }
}

std::ostream & operator <<( std::ostream& out, const DataEntryUrnBox& urn ) {
    out << "version=" << int(urn.m_version) << " flags=" << std::hex << urn.m_flags << std::dec;
    out << " name='" << urn.m_name << "' location='" << urn.m_location << "'";
    return out;
}

DataEntryUrnBox::DataEntryUrnBox( std::ifstream& f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    uint32_t off{0};
    for( ; off < (sz - 3*sizeof(uint32_t)); ++off ) {
        char c = f.get();
        if( c == 0 )
            break;
        m_name += c;
    }
    for( ; off < (sz - 3*sizeof(uint32_t)); ++off )
        m_location += f.get();
}

std::ostream & operator <<( std::ostream& out, const DataReferenceBox& dref ) {
    out << "version=" << int(dref.m_version) << " flags=" << std::hex << dref.m_flags << std::dec;
    out << " item count=" << dref.m_entries.size();
    return out;
}
DataReferenceBox::DataReferenceBox( std::ifstream& f, uint32_t sz )
{
    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    uint32_t count{0};
    f.read( (char*)&count, sizeof(count) );
    count = be32toh( count );

    m_entries.resize( count );
}
