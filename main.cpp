#include "filetypebox.hh"
#include "movieheaderbox.hh"
#include "trackheaderbox.hh"
#include "mediaheaderbox.hh"
#include "handlerreferencebox.hh"
#include "videomediaheaderbox.hh"
#include "soundmediaheaderbox.hh"
#include "datareferencebox.hh"
#include "timetosamplebox.hh"
#include "compositionoffsetbox.hh"
#include "sampledescriptionbox.hh"
#include "avccbox.hh"
#include "samplesizebox.hh"
#include "sampletochunkbox.hh"
#include "movieextendsheaderbox.hh"
#include "trackextendsbox.hh"
#include "moviefragmentheaderbox.hh"
#include "trackfragmentheaderbox.hh"
#include "trackfragmentrunbox.hh"
#include "mediadatabox.hh"
#include "elementarystreamdescriptorbox.h"
#include <string.h>

#include <iostream>
#include <fstream>

bool isAtom( const char* atom, const char* type ) {
    return strcmp( atom, type ) == 0;
}
bool isContainer( const char* type ) {
    return isAtom( "moov", type ) ||
           isAtom( "edts", type ) ||
           isAtom( "trak", type ) ||
           isAtom( "mdia", type ) ||
           isAtom( "minf", type ) ||
           isAtom( "dinf", type ) ||
           isAtom( "stbl", type ) ||
           isAtom( "mvex", type ) ||
           isAtom( "moof", type ) ||
           isAtom( "traf", type ) ||
           isAtom( "udta", type );
}

uint32_t getBox( std::ifstream& f, char* type ) {
    uint32_t rc;
    f.read( (char*)&rc, sizeof( rc ) );
    f.read( type, 4 );
    fprintf(stderr,"%02x %02x %02x %02x\n", type[0],type[1],type[2],type[3]);

    return be32toh( rc );
}

void qqq( std::ifstream& f, size_t sz ) {
    size_t pos = f.tellg();
    std::vector< char > buf( sz );
    f.read( buf.data(), buf.size() );

    std::ofstream ff( "fmp4.trun" );
    if( ff.is_open() ) {
        ff.write( buf.data(), buf.size() );
        ff.close();
    }
    f.seekg( pos );
}

int main( int argc, char* argv[] ) {
    if( argc != 2 ) {
        std::cerr << "run as " << argv[0] << " file to verify\n";
        return 1;
    }
    std::ifstream f( argv[1], std::ios_base::ate );
    long fullsz = f.tellg();
    f.close();

    f.open( argv[1] );
    if( !f.is_open() ) {
        std::cerr << "failed to open " << argv[1] << std::endl;
        return 1;
    }

    size_t moof_position = 0;
    uint32_t trackID = 0;
    TrunMap trun_map;
    std::string handlerType;

    while( f.good() ) {
        char type[ 5 ] = { 0,0,0,0,0};
        uint32_t size = getBox( f, type );
        std::cerr << "atom: type=" << type << " size=" << size << std::endl;

        if( isContainer( type ) ) {
            if( isAtom( "moof", type ) )
               moof_position = f.tellg();
	}
	else {
            long pos = f.tellg();
            if( isAtom( "ftyp", type ) ) {
                std::cerr << FileTypeBox( f, size ) << std::endl;
            }
            else if( isAtom( "mvhd", type ) ) {
                std::cerr << MovieHeaderBox( f, size ) << std::endl;
            }
            else if( isAtom( "tkhd", type ) ) {
                std::cerr << TrackHeaderBox( f, size ) << std::endl;
            }
            else if( isAtom( "mdhd", type ) ) {
                std::cerr << MediaHeaderBox( f, size ) << std::endl;
            }
            else if( isAtom( "hdlr", type ) ) {
                HandlerReferenceBox hdlr( f, size );
                handlerType = hdlr.type();

                std::cerr << hdlr << std::endl;
            }
            else if( isAtom( "vmhd", type ) ) {
                std::cerr  << VideoMediaHeaderBox( f, size ) << std::endl;
            }
            else if( isAtom( "smhd", type ) ) {
                std::cerr  << SoundMediaHeaderBox( f, size ) << std::endl;
            }
            else if( isAtom( "dref", type ) ) {
                std::cerr  << DataReferenceBox( f, size ) << std::endl;
            }
            else if( isAtom( "url ", type ) ) {
                std::cerr  << DataEntryUrlBox( f, size ) << std::endl;
            }
            else if( isAtom( "urn ", type ) ) {
                std::cerr  << DataEntryUrnBox( f, size ) << std::endl;
            }
            else if( isAtom( "stts", type ) ) {
                std::cerr  << TimeToSampleBox( f, size ) << std::endl;
            }
            else if( isAtom( "ctts", type ) ) {
                std::cerr  << CompositionOffsetBox( f, size ) << std::endl;
            }
            else if( isAtom( "stsd", type ) ) {
                std::cerr  << SampleDescriptionBox( f, size, handlerType ) << std::endl;
            }
            else if( isAtom( "avcC", type ) ) {
                std::cerr  << AvcCBox( f, size ) << std::endl;
            }
            else if( isAtom( "stsz", type ) ) {
                std::cerr  << SampleSizeBox( f, size ) << std::endl;
            }
            else if( isAtom( "stsc", type ) ) {
                std::cerr  << SampleToChunkBox( f, size ) << std::endl;
            }
            else if( isAtom( "mehd", type ) ) {
                std::cerr  << MovieExtendsHeaderBox( f, size ) << std::endl;
            }
            else if( isAtom( "trex", type ) ) {
                std::cerr  << TrackExtendsBox( f, size ) << std::endl;
            }
            else if( isAtom( "mfhd", type ) ) {
                std::cerr  << MovieFragmentHeaderBox( f, size ) << std::endl;
            }
            else if( isAtom( "tfhd", type ) ) {
                TrackFragmentHeaderBox tfhd( f, size );
		trackID = tfhd.trackID();
                std::cerr  << tfhd << std::endl;
            }
            else if( isAtom( "trun", type ) ) {
                qqq( f, size );
                auto rc = trun_map.emplace( trackID, new TrackFragmentRunBox( f, size ) );
                std::cerr  << *(rc.first->second) << std::endl;
            }
            else if( isAtom( "mdat", type ) ) {
                std::cerr  << MediaDataBox( f, moof_position, trun_map, size ) << std::endl;
            }
            else if( isAtom( "esds", type ) ) {
                std::cerr << ElementaryStreamDescriptorBox( f, size ) << std::endl;
            }
            else if( isAtom( "hvcC", type ) ) {
                for( size_t i(0); i < size - 8; ++i ) fprintf( stderr, "%02x ", f.get() );
		fprintf( stderr, " : payload size = %d\n", size - 8 );
	    }
            else
                f.seekg( pos + (size - 8) );
        }
        std::cerr << "pos: " << f.tellg() << std::endl;
        if( f.tellg() >= fullsz )
            break;

        getchar();
    }

    f.close();
    return 0;
}
