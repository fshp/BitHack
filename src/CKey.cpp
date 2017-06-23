#include "CKey.hpp"
#include <boost/format.hpp>
#include <iostream>

namespace BitHack {

CKey::CKey(const privKey_t &privateKey, const ripemd160_t &addrUncomp, const ripemd160_t &addrComp) :
    m_privateKey(privateKey),
    m_addrUncomp(addrUncomp),
    m_addrComp(addrComp) {
}

std::ostream& operator <<(std::ostream &os, const CKey &key) {
    boost::format formater("%02x");
    auto printer = [&formater](const uint16_t c){std::cout << formater % c;};

    std::for_each(std::begin(key.m_privateKey), std::end(key.m_privateKey), printer);
    os << "\t";
    std::for_each(std::begin(key.m_addrUncomp), std::end(key.m_addrUncomp), printer);
    os << "\t";
    std::for_each(std::begin(key.m_addrComp), std::end(key.m_addrComp), printer);
    return os;
}

} /* namespace BitHack */

