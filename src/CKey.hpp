#ifndef CKEY_HPP_
#define CKEY_HPP_

#include <array>
#include <ostream>

namespace BitHack {

using privateKey_t       = std::array<unsigned char, 32>;
using addressRipemd160_t = std::array<unsigned char, 20>;

class CKey {
public:
    friend class CKeyFactory;
    friend std::ostream& operator <<(std::ostream &os, const CKey &key);

protected:
    CKey(const privateKey_t &privateKey, const addressRipemd160_t &addressRipemd160);

    privateKey_t      m_privateKey;
    addressRipemd160_t m_addressRipemd160;
};

} /* namespace BitHack */

#endif /* CKEY_HPP_ */
