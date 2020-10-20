#include "atom.h"
#include "trackfragmentrunbox.hh"
#include "mediadatabox.hh"
#include "trackfragmentheaderbox.hh"

#include <fstream>
#include <iostream>

int main( int argc, char* argv[] ) {
    if( argc != 2 ) {
        std::cerr << "run as " << argv[0] << " file to verify\n";
        return 1;
    }
    std::ifstream f( argv[1], std::ios_base::ate );
    if( !f.is_open() ) {
        std::cerr << "failed to open " << argv[1] << std::endl;
        return 1;
    }
    std::streampos filesize = f.tellg();
    f.seekg( 0 );

    uint32_t track_id = 0;
    TrunMap trun_map;
    std::unique_ptr< MediaDataBox > data;

    while( f.good() ) {
        std::unique_ptr< Atom > atom( Atom::make( f, trun_map ) );
        std::cout << (*atom) << std::endl;
        if( atom ) {
            if( *atom == Atom::tfhd ) {
                track_id = static_cast< TrackFragmentHeaderBox* >(atom.get())->trackID();
            }
            else if( *atom == Atom::trun ) {
                trun_map[track_id] = std::shared_ptr< TrackFragmentRunBox >( static_cast< TrackFragmentRunBox* >(atom.release()) );
            }
        }
        if( f.tellg() >= filesize )
            break;
    }
    f.close();

    return 0;
}
