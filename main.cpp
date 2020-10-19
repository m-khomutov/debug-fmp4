#include "atom.h"
#include "trackextendsbox.hh"
#include "trackfragmentrunbox.hh"
#include "mediadatabox.hh"
#include "trackfragmentheaderbox.hh"
#include "trackfragmentrunbox.hh"
#include <string.h>

#include <iostream>

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
    uint32_t track_id = 0;
    TrunMap trun_map;

    while( f.good() ) {
        std::unique_ptr< Atom > atom( Atom::make( f ) );
        if( atom ) {
            std::cout << (*atom) << std::endl;
            if( *atom == Atom::tfhd ) {
                track_id = static_cast< TrackFragmentHeaderBox* >(atom.get())->trackID();
            }
            else if( *atom == Atom::trun ) {
                auto rc = trun_map.emplace( track_id, std::shared_ptr< TrackFragmentRunBox >( static_cast< TrackFragmentRunBox* >(atom.release()) ) );
            }
        }
        /*Atom atom( f );
        std::cout << atom << std::endl;
        if( atom.container() ) {
            if( atom == Atom::moof )
               moof_position = f.tellg();
        }
        else {
            else if( atom == Atom::mdat ) {
                if( !trun_map.empty() ) {
                    std::cout  << MediaDataBox( f, moof_position, trun_map, atom.size() ) << std::endl;
                }
                else {
                    f.seekg( atom.position() + std::streampos(atom.size()) );
                }
            }
            else
                f.seekg( atom.position() + std::streampos(atom.size()) );
        }
        std::cout << "pos: " << f.tellg() << std::endl;
        if( f.tellg() >= fullsz )
            break;
*/
        //getchar();
    }

    f.close();
    return 0;
}
