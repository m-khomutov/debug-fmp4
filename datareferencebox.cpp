#include "datareferencebox.hh"

DataEntryBox::DataEntryBox( std::istream& is ) : Atom( is )
{}

DataEntryUrlBox::DataEntryUrlBox( std::istream& is ) : DataEntryBox( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    if( m_flags != 0x000001 ) {
        for( uint32_t i(0); i < (size() - 3*sizeof(uint32_t)); ++i )
        m_location += is.get();
    }
}

void DataEntryUrlBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " location='" << m_location << "'";
}

DataEntryUrnBox::DataEntryUrnBox( std::istream& is ) : DataEntryBox( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    uint32_t off{0};
    for( ; off < (size() - 3*sizeof(uint32_t)); ++off ) {
        char c = is.get();
        if( c == 0 )
            break;
        m_name += c;
    }
    for( ; off < (size() - 3*sizeof(uint32_t)); ++off )
        m_location += is.get();
}

void DataEntryUrnBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    indent( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " name='" << m_name << "' location='" << m_location << "'";
}

DataReferenceBox::DataReferenceBox( std::istream& is ) : Atom( is ) {
    uint32_t tmp;
    is.read( (char*)&tmp, sizeof(tmp) );
    m_version = tmp & 0xff;
    m_flags = tmp >> 24;

    uint32_t count{0};
    is.read( (char*)&count, sizeof(count) );
    count = be32toh( count );

    m_entries.resize( count );
}

void DataReferenceBox::fout( std::ostream &out ) const {
    Atom::fout( out );
    indent( out );
    out << "version=" << int(m_version) << " flags=" << std::hex << m_flags << std::dec;
    out << " item count=" << m_entries.size();
}
