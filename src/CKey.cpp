#include "CKey.hpp"
#include <boost/format.hpp>
#include <iostream>

namespace BitHack {

CKey::CKey(const privateKey_t &privateKey, const addressRipemd160_t &addressRipemd160) :
        m_privateKey(privateKey), m_addressRipemd160(addressRipemd160) {
}

std::ostream& operator <<(std::ostream &os, const CKey &key) {
    boost::format formater("%02x");
    auto printer = [&formater](const uint16_t c){std::cout << formater % c;};

    std::for_each(std::begin(key.m_privateKey), std::end(key.m_privateKey), printer);
    os << "\t\t";
    std::for_each(std::begin(key.m_addressRipemd160), std::end(key.m_addressRipemd160), printer);
    os << std::endl;
    return os;
}

} /* namespace BitHack */

