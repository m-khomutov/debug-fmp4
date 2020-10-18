#include "avccbox.hh"

#include <iostream>

std::ostream & operator <<( std::ostream& out, const AvcCBox& avcc ) {
    out << std::hex;
    out << "version=" << int(avcc.m_version) << " profile=0x" << int(avcc.m_profile) << " compat=0x" << int(avcc.m_compatibility)
        << " level=0x" << int(avcc.m_level) << " nalu length=" << int(avcc.m_nalulen) << " num of SPS=" << avcc.m_sps.size();

    out << "\nSPS: ";
    for( auto sps: avcc.m_sps ) {
        out << "{ ";
        for( auto b : sps )
            out << int(b) << " ";
        out << "}";
    }
    out << "\nPPS: ";
    for( auto pps: avcc.m_pps ) {
        out << "{ ";
        for( auto b : pps )
            out << int(b) << " ";
        out << "}";
    }
    out << std::dec;

    return out;
}

AvcCBox::AvcCBox( std::ifstream& f, uint32_t sz )
{
    m_version = f.get();
    m_profile = f.get();
    m_compatibility = f.get();
    m_level = f.get();
    m_nalulen = ((f.get())&0x3) + 1;  // 6 reserved ( all bits on ) + 2 NALULengthSizeMinusOne

    uint8_t numOfSps = (f.get())&0x1f;  //  3 reserved ( all bits on ) + 5 number of SPS NALUs (usually 1)

    for( uint8_t i(0); i < numOfSps; ++i ) {
        uint16_t len;
        f.read( (char*)&len, sizeof(len) );
        std::cerr << "sps len: " << std::hex << len << " ";
        len = be16toh( len );
        m_sps.push_back( std::vector< uint8_t >(len) );
        f.read( (char*)m_sps.back().data(), len );
    }

    uint8_t numOfPps = (f.get())&0x1f;  //  3 reserved ( all bits on ) + 5 number of SPS NALUs (usually 1)
    for( uint8_t i(0); i < numOfPps; ++i ) {
        uint16_t len;
        f.read( (char*)&len, sizeof(len) );
        len = be16toh( len );
        std::cerr << "pps len: " << len << std::dec <<std::endl;

        m_pps.push_back( std::vector< uint8_t >(len) );
        f.read( (char*)m_pps.back().data(), len );
    }
}
