#ifndef HANDLERREFERENCEBOX_HH
#define HANDLERREFERENCEBOX_HH

#include "atom.h"

class HandlerReferenceBox : public Atom {
public:
    HandlerReferenceBox( std::istream & is, std::string * type );

    Atom::Value type() const {
        return m_type;
    }

private:
    uint8_t m_version;
    uint32_t m_flags;

    Atom::Value m_type;
    std::string m_typeAsString;
    std::string m_name;

private:
    void fout( std::ostream& out ) const override;
};

#endif // HANDLERREFERENCEBOX_HH
