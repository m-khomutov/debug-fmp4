#ifndef HANDLERREFERENCEBOX_HH
#define HANDLERREFERENCEBOX_HH

#include "atom.h"

class HandlerReferenceBox : public Atom {
public:
    HandlerReferenceBox( std::istream & is, std::string * type );

    const std::string& type() const {
        return m_handlerType;
    }

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::string m_handlerType;
    std::string m_name;

private:
    void fout( std::ostream& out ) const override;
};

#endif // HANDLERREFERENCEBOX_HH
