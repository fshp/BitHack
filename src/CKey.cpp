#include "CKey.hpp"
#include <boost/format.hpp>

namespace BitHack {

CKey::CKey() {
}

CKey::CKey(const array<unsigned char, 32> &privateKey,
        const array<unsigned char, 20> &addressRipemd160) :
        m_privateKey(privateKey), m_addressRipemd160(addressRipemd160) {
}

CKey::~CKey() {
    // TODO Auto-generated destructor stub
}

ostream& operator <<(ostream &os, const CKey &key) {
    os << "Private key: ";
    for (auto p : key.m_privateKey)
        os << boost::format("%02x") % static_cast<uint16_t>(p);
    os << endl;
    os << "Address in Ripemd160: ";
    for (auto p : key.m_addressRipemd160)
        os << boost::format("%02x") % static_cast<uint16_t>(p);
    os << endl;
    return os;
}

} /* namespace BitHack */

