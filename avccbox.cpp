#include <iomanip>
#include "avccbox.hh"

AvcCBox::AvcCBox( std::istream& is ) : Atom( is ) {
    m_version = is.get();
    m_profile = is.get();
    m_compatibility = is.get();
    m_level = is.get();
    m_nalulen = ((is.get())&0x3) + 1;  // 6 reserved ( all bits on ) + 2 NALULengthSizeMinusOne

    uint8_t numOfSps = (is.get())&0x1f;  //  3 reserved ( all bits on ) + 5 number of SPS NALUs (usually 1)

    for( uint8_t i(0); i < numOfSps; ++i ) {
        uint16_t len;
        is.read( (char*)&len, sizeof(len) );
        len = be16toh( len );
        m_sps.push_back( std::vector< uint8_t >(len) );
        is.read( (char*)m_sps.back().data(), len );
    }

    uint8_t numOfPps = (is.get())&0x1f;  //  3 reserved ( all bits on ) + 5 number of SPS NALUs (usually 1)
    for( uint8_t i(0); i < numOfPps; ++i ) {
        uint16_t len;
        is.read( (char*)&len, sizeof(len) );
        len = be16toh( len );

        m_pps.push_back( std::vector< uint8_t >(len) );
        is.read( (char*)m_pps.back().data(), len );
    }
}

void AvcCBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << std::hex;
    out << "\nversion=" << int(m_version) << " profile=0x" << int(m_profile) << " compat=0x" << int(m_compatibility)
        << " level=0x" << int(m_level)
        << std::dec << " nalu length=" << int(m_nalulen) << " num of SPS=" << m_sps.size();

    out << "\nSPS: " << std::hex << std::setw(2) << std::setfill('0');
    for( auto sps: m_sps ) {
        out << "{ ";
        for( auto b : sps )
            out << int(b) << " ";
        out << "}";
    }
    out << "\nPPS: ";
    for( auto pps: m_pps ) {
        out << "{ ";
        for( auto b : pps )
            out << int(b) << " ";
        out << "}";
    }
    out << std::dec;
}
