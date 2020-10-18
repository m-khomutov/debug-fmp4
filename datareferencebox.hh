#ifndef DATAREFERENCEBOX_HH
#define DATAREFERENCEBOX_HH

#include <fstream>
#include <string>
#include <vector>
#include <memory>

class DataEntryBox {
public:
    virtual ~DataEntryBox() = default;

    friend std::ostream & operator <<( std::ostream& out, const DataEntryBox& vmhd );
};

class DataEntryUrlBox : public DataEntryBox {
public:
    DataEntryUrlBox( std::ifstream& f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::string m_location;

    friend std::ostream & operator <<( std::ostream& out, const DataEntryUrlBox& vmhd );
};

class DataEntryUrnBox : public DataEntryBox {
public:
    DataEntryUrnBox( std::ifstream& f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::string m_name;
    std::string m_location;

    friend std::ostream & operator <<( std::ostream& out, const DataEntryUrnBox& vmhd );
};

class DataReferenceBox
{
public:
    DataReferenceBox( std::ifstream& f, uint32_t sz );

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::vector< std::shared_ptr< DataEntryBox > > m_entries;

    friend std::ostream & operator <<( std::ostream& out, const DataReferenceBox& dref );
};

#endif // DATAREFERENCEBOX_HH
