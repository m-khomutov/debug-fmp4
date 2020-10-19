//
// Created by mkh on 19.11.2019.
//

#include "elementarystreamdescriptorbox.h"

ElementaryStreamDescriptorBox::Descriptor* ElementaryStreamDescriptorBox::Descriptor::make( std::istream & is ) {
    char buf[4];
    is.read( buf, 2 );
    uint8_t length = buf[1];
    if( length == 0x80 ) {  // 3 bytes extended descriptor type tag Start = 0x80 ; End = 0xFE NOTE: the extended start tags may be left out
        is.read( &buf[1], 3 );
        length = buf[3];
    }
    switch( buf[0] ) {
        case Descriptor::kElementaryStream:
            return new ElementaryStreamDescriptor( is, length );
        case Descriptor::kDecoderConfig:
            return new DecoderConfigDescriptor( is, length );
        case Descriptor::kDecoderSpecific:
            return new DecoderSpecificDescriptor( is, length );
        case Descriptor::kSlConfig:
            return new SlConfigDescriptor( is, length );
    }
    return nullptr;
}

ElementaryStreamDescriptorBox::ElementaryStreamDescriptor::ElementaryStreamDescriptor( std::istream& is, uint8_t len )
: Descriptor( kElementaryStream, len ) {
    is.read( (char*)&m_id, sizeof(m_id) );
    m_id = be16toh( m_id );
    m_priority = is.get();
}
void ElementaryStreamDescriptorBox::ElementaryStreamDescriptor::fout( std::ostream &out ) const {
    out << "id=" << m_id << " priority=" << int(m_priority) << " ";
}

ElementaryStreamDescriptorBox::DecoderConfigDescriptor::DecoderConfigDescriptor( std::istream &is, uint8_t len )
: Descriptor( kDecoderConfig, len ) {
    m_object_type = is.get();
    uint8_t c = is.get();
    m_stream_type = (c >> 2);
    m_upstream = (c >> 1) & 1;

    for( int i(0); i < 3; ++i ) // buffer size = 24-bit unsigned value
        is.get();
    is.read( (char*)&m_maximum_bitrate, sizeof(m_maximum_bitrate) );
    m_maximum_bitrate = be32toh( m_maximum_bitrate );
    is.read( (char*)&m_average_bitrate, sizeof(m_average_bitrate) );
    m_average_bitrate = be32toh( m_average_bitrate );
}
void ElementaryStreamDescriptorBox::DecoderConfigDescriptor::fout( std::ostream &out ) const {
    out << "object='";
    switch( m_object_type ) {
        case 1:
            out << "system v1' ";
            break;
        case 2:
            out << "system v2' ";
            break;
        case 32:
            out << "MPEG-4 video' ";
            break;
        case 33:
            out << "MPEG-4 AVC SPS' ";
            break;
        case 34:
            out << "MPEG-4 AVC PPS' ";
            break;
        case 64:
            out << "MPEG-4 audio' ";
            break;
        case 96:
            out << "MPEG-2 simple video' ";
            break;
        case 97:
            out << "MPEG-2 main video' ";
            break;
        case 98:
            out << "MPEG-2 SNR video' ";
            break;
        case 99:
            out << "MPEG-2 spatial video' ";
            break;
        case 100:
            out << "MPEG-2 high video' ";
            break;
        case 101:
            out << "MPEG-2 4:2:2 video' ";
            break;
        case 102:
            out << "MPEG-4 ADTS main' ";
            break;
        case 103:
            out << "MPEG-4 ADTS Low Complexity' ";
            break;
        case 104:
            out << "MPEG-4 ADTS Scalable Sampling Rate' ";
            break;
        case 105:
            out << "MPEG-2 ADTS' ";
            break;
        case 106:
            out << "MPEG-1 video' ";
            break;
        case 107:
            out << "MPEG-1 ADTS' ";
            break;
        case 108:
            out << "JPEG video' ";
            break;
        case 192:
            out << "private audio' ";
            break;
        case 208:
            out << "private video' ";
            break;
        case 224:
            out << "16-bit PCM LE audio' ";
            break;
        case 225:
            out << "vorbis audio' ";
            break;
        case 226:
            out << "dolby v3 (AC3) audio' ";
            break;
        case 227:
            out << "alaw audio' ";
            break;
        case 228:
            out << "mulaw audio' ";
            break;
        case 229:
            out << "G723 ADPCM audio' ";
            break;
        case 230:
            out << "16-bit PCM Big Endian audio' ";
            break;
        case 240:
            out << "Y'CbCr 4:2:0 (YV12) video' ";
            break;
        case 241:
            out << "H264 video' ";
            break;
        case 242:
            out << "H263 video' ";
            break;
        case 243:
            out << "H261 video' ";
            break;
    }
    out << "stream='";
    switch( m_stream_type ) {
        case 1:
            out << "object descript.' ";
            break;
        case 2:
            out << "clock ref.' ";
            break;
        case 4:
            out << "scene descript.' ";
            break;
        case 5:
            out << "audio' ";
            break;
        case 6:
            out << "MPEG-7' ";
            break;
        case 7:
            out << "IPMP' ";
            break;
        case 8:
            out << "OCI' ";
            break;
        case 9:
            out << "MPEG Java' ";
            break;
        case 32:
            out << "user private' ";
            break;
    }
    out << "upstream=" << std::boolalpha << m_upstream << " maxbitrate=" << m_maximum_bitrate
        << " avgbitrate=" << m_average_bitrate;
}

ElementaryStreamDescriptorBox::DecoderSpecificDescriptor::DecoderSpecificDescriptor( std::istream &is, uint8_t len )
: Descriptor( kDecoderSpecific, len ) {
    m_start_codes.resize( size() );
    is.read( (char*)m_start_codes.data(), size() );
}
void ElementaryStreamDescriptorBox::DecoderSpecificDescriptor::fout( std::ostream &out ) const {
    out << " start codes={";
    for( auto c : m_start_codes )
        out << std::hex << " " << int(c);
    out << " }" << std::dec << " ";
}

ElementaryStreamDescriptorBox::SlConfigDescriptor::SlConfigDescriptor( std::istream& is, uint8_t len )
: Descriptor( kSlConfig, len ) {
    m_value = is.get();  // SL value = 8-bit hex value set to 0x02
}
void ElementaryStreamDescriptorBox::SlConfigDescriptor::fout( std::ostream &out ) const {
    out << "SLvalue=" << int(m_value);
}

ElementaryStreamDescriptorBox::ElementaryStreamDescriptorBox( std::istream& is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp >> 24;
    m_flags = tmp & 0xffffff;

    for( uint8_t i(3); i <= 6; ++i ) {
        m_descriptors.push_back( std::shared_ptr< Descriptor >( Descriptor::make( is ) ) );
    }
}

void ElementaryStreamDescriptorBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "\nversion=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec << " ";
    for( auto d : m_descriptors )
        d->fout( out );
}