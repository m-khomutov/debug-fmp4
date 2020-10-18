#ifndef HANDLERREFERENCEBOX_HH
#define HANDLERREFERENCEBOX_HH

#include <fstream>
#include <string>

class HandlerReferenceBox
{
public:
    HandlerReferenceBox( std::ifstream & f, uint32_t sz );

    const std::string& type() const {
        return m_handlerType;
    }

private:
    uint8_t m_version;
    uint32_t m_flags;

    std::string m_handlerType;
    std::string m_name;

    friend std::ostream & operator <<( std::ostream& out, const HandlerReferenceBox& mvhd );
};

#endif // HANDLERREFERENCEBOX_HH
