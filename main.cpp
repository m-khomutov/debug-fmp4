#include "atom.h"
#include "trackfragmentrunbox.hh"
#include "mediadatabox.hh"
#include "trackfragmentheaderbox.hh"

#include <fstream>
#include <iostream>

namespace {
    class Indent {
    public:
        Indent( int step =3 ) : m_step( step ) {}

        operator int() const {
            return m_items.empty() ? 0 : m_items.back().indent;
        }

        void add( std::streampos size, std::streampos unserialized ) {
            int indent = m_items.empty() ? 0 : m_items.back().indent + m_step;
            m_items.emplace_back( size, unserialized, indent );
        }
        void normalize() {
            if( !m_items.empty() ) {
                std::vector< Item >::reverse_iterator it = m_items.rbegin();
                while( it != m_items.rend() ) {
                    if( it->unserialized == 0 ) {
                        std::vector< Item >::reverse_iterator next_it = std::next( it );
                        if( next_it != m_items.rend() )
                            next_it->unserialized -= it->size;
                        if( it.base() != m_items.end() ) {
                            m_items.erase( it.base() );
                        }
                        else {
                            m_items.pop_back();
                        }
                        it = next_it;
                    }
                    else
                        it++;
                }
            }
        }

    private:
        struct Item {
            std::streampos size;
            std::streampos unserialized;
            int indent;

            Item() = default;
            Item( std::streampos s, std::streampos u, int i ) : size( s ),unserialized( u ),indent( i )
            {}
        };

        int m_step;
        std::vector< Item > m_items;
    };
}  // namespace

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
    Indent indent;

    while( f.good() ) {
        std::unique_ptr< Atom > atom( Atom::make( f, trun_map ) );
        if( atom ) {
            indent.add( atom->size(), std::streampos(atom->size()) - (f.tellg() - atom->position()) );
            atom->setIndent( indent );
            indent.normalize();

            std::cout << (*atom) << std::endl;
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
