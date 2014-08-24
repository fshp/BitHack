#include "CKey.hpp"
#include <boost/format.hpp>
#include <functional>
#include <utility>

namespace BitHack {

CKey::CKey() {
}

CKey::CKey(CKey&& other) :
        m_privateKey(move(other.m_privateKey)),
        m_addressRipemd160(move(other.m_addressRipemd160)) {
}

CKey::CKey(const array<unsigned char, 32> &privateKey,
        const array<unsigned char, 20> &addressRipemd160) :
        m_privateKey(privateKey), m_addressRipemd160(addressRipemd160) {
}

ostream& operator <<(ostream &os, const CKey &key) {
    boost::format formater("%02x");
    auto printer = [&formater](const uint16_t c){cout << formater % c;};

    for_each(begin(key.m_privateKey), end(key.m_privateKey), printer);
    os << "\t\t";
    for_each(begin(key.m_addressRipemd160), end(key.m_addressRipemd160), printer);
    os << endl;
    return os;
}

} /* namespace BitHack */

