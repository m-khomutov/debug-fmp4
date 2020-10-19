#ifndef DATAREFERENCEBOX_HH
#define DATAREFERENCEBOX_HH

#include "atom.h"
#include <vector>
#include <memory>

class DataEntryBox : public Atom {
public:
    DataEntryBox( std::istream & is );

    virtual ~DataEntryBox() = default;
};

class DataEntryUrlBox : public DataEntryBox {
public:
    DataEntryUrlBox( std::istream& is );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::string m_location;

private:
    void fout( std::ostream& out ) const override;
};

class DataEntryUrnBox : public DataEntryBox {
public:
    DataEntryUrnBox( std::istream& is );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::string m_name;
    std::string m_location;

private:
    void fout( std::ostream& out ) const override;
};

class DataReferenceBox : public Atom {
public:
    DataReferenceBox( std::istream& f );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::shared_ptr< DataEntryBox > > m_entries;

private:
    void fout( std::ostream& out ) const override;
};

#endif // DATAREFERENCEBOX_HH
