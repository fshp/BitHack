#ifndef CKEY_HPP_
#define CKEY_HPP_

#include <array>
#include <ostream>

namespace BitHack {

using privKey_t       = std::array<unsigned char, 32>;
using ripemd160_t = std::array<unsigned char, 20>;

class CKey {
public:
    friend class CKeyFactory;
    friend std::ostream& operator <<(std::ostream &os, const CKey &key);

protected:
    CKey(const privKey_t &privateKey, const ripemd160_t &addrUncomp, const ripemd160_t &m_addrComp);

    privKey_t      m_privateKey;
    ripemd160_t m_addrUncomp;
    ripemd160_t m_addrComp;
};

} /* namespace BitHack */

#endif /* CKEY_HPP_ */
