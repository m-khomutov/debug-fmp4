//
// Created by mkh on 19.11.2019.
//

#include "elementarystreamdescriptorbox.h"
#include <fstream>

std::ostream & operator <<( std::ostream& out, const ElementaryStreamDescriptorBox& esds ) {
    out << "ES ID=" << esds.m_esId << " object type ID=";
    switch( esds.m_objectType ) {
        case 1:
            out << "system v1 ";
            break;
        case 2:
            out << "system v2 ";
            break;
        case 32:
            out << "MPEG-4 video ";
            break;
        case 33:
            out << "MPEG-4 AVC SPS ";
            break;
        case 34:
            out << "MPEG-4 AVC PPS ";
            break;
        case 64:
            out << "MPEG-4 audio ";
            break;
        case 96:
            out << "MPEG-2 simple video ";
            break;
        case 97:
            out << "MPEG-2 main video ";
            break;
        case 98:
            out << "MPEG-2 SNR video ";
            break;
        case 99:
            out << "MPEG-2 spatial video ";
            break;
        case 100:
            out << "MPEG-2 high video ";
            break;
        case 101:
            out << "MPEG-2 4:2:2 video ";
            break;
        case 102:
            out << "MPEG-4 ADTS main ";
            break;
        case 103:
            out << "MPEG-4 ADTS Low Complexity ";
            break;
        case 104:
            out << "MPEG-4 ADTS Scalable Sampling Rate ";
            break;
        case 105:
            out << "MPEG-2 ADTS ";
            break;
        case 106:
            out << "MPEG-1 video ";
            break;
        case 107:
            out << "MPEG-1 ADTS ";
            break;
        case 108:
            out << "JPEG video ";
            break;
        case 192:
            out << "private audio ";
            break;
        case 208:
            out << "private video ";
            break;
        case 224:
            out << "16-bit PCM LE audio ";
            break;
        case 225:
            out << "vorbis audio ";
            break;
        case 226:
            out << "dolby v3 (AC3) audio ";
            break;
        case 227:
            out << "alaw audio ";
            break;
        case 228:
            out << "mulaw audio ";
            break;
        case 229:
            out << "G723 ADPCM audio ";
            break;
        case 230:
            out << "16-bit PCM Big Endian audio ";
            break;
        case 240:
            out << "Y'CbCr 4:2:0 (YV12) video ";
            break;
        case 241:
            out << "H264 video ";
            break;
        case 242:
            out << "H263 video ";
            break;
        case 243:
            out << "H261 video ";
            break;
    }
    out << "stream type=";
    switch( esds.m_streamType ) {
        case 1:
            out << "object descript. ";
            break;
        case 2:
            out << "clock ref. ";
            break;
        case 4:
            out << "scene descript. ";
            break;
        case 5:
            out << "audio ";
            break;
        case 6:
            out << "MPEG-7 ";
            break;
        case 7:
            out << "IPMP ";
            break;
        case 8:
            out << "OCI ";
            break;
        case 9:
            out << "MPEG Java ";
            break;
        case 32:
            out << "user private ";
            break;
    }
    out << "upstream flag=" << std::boolalpha << esds.m_upstream << " maximum bit rate=" << esds.m_maximumBitrate
        << " average bit rate=" << esds.m_averageBitrate << " ES header start codes={";
    for( auto c : esds.m_esHeaderStartCodes )
        out << std::hex << " " << int(c);
    out << " }" << std::dec;

    return out;
}

ElementaryStreamDescriptorBox::ElementaryStreamDescriptorBox( std::ifstream &f, uint32_t sz ) {
    size_t pos = f.tellg();

    uint32_t tmp;
    f.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    for( uint8_t i(3); i <= 6; ++i ) {
        if( !f_get_tag( f, i ) ) {
            f.seekg( pos + (sz - 8) );
            return;
        }
    }
}

#include <iostream>
bool ElementaryStreamDescriptorBox::f_get_tag( std::ifstream &f, uint8_t tagType ) {
    char buf[4];
    f.read( buf, sizeof(buf) );
    if( buf[0] != tagType )
        return false;

    switch( tagType ) {
        case 3: {
            f.get(); // tag length
            f.read( (char*)&m_esId, sizeof(m_esId) );
            m_esId = be16toh( m_esId );
            std::cerr << "ES ID: " << m_esId << std::endl;
            f.get(); // stream priority;
            break;
        }
        case 4: {
            f.get();  // tag length
            m_objectType = f.get();
            std::cerr << "obj type:" << int(m_objectType) << std::endl;

            uint8_t c = f.get();
            m_streamType = (c >> 2);
            std::cerr << "stream type:" << int(m_streamType) << std::endl;
            m_upstream = (c >> 1) & 1;
            std::cerr << "upstream:" << m_upstream << std::endl;

            f.read( buf, 3 ); // buffer size = 24-bit unsigned value

            f.read( (char*)&m_maximumBitrate, sizeof(m_maximumBitrate) );
            m_maximumBitrate = be32toh( m_maximumBitrate );

            f.read( (char*)&m_averageBitrate, sizeof(m_averageBitrate) );
            m_averageBitrate = be32toh( m_averageBitrate );
            break;
        }
        case 5: {
            uint8_t c = f.get();  // tag length
            m_esHeaderStartCodes.resize( c );
            f.read( (char*)m_esHeaderStartCodes.data(), c );
            break;
        }
        case 6: {
            f.get();  // tag length
            f.get();  // SL value = 8-bit hex value set to 0x02
            break;
        }
    }
    return true;
}