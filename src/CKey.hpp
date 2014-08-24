#ifndef CKEY_HPP_
#define CKEY_HPP_

#include <array>
#include <ostream>

namespace BitHack {

using namespace std;

class CKey {
    friend class CKeyFactory;
public:
    CKey(CKey&& other);
    friend ostream& operator <<(ostream &os, const CKey &key);

protected:
    CKey();
    CKey(const CKey &other) = delete;
    CKey(const array<unsigned char, 32> &privateKey,
            const array<unsigned char, 20> &addressRipemd160);
    array<unsigned char, 32> m_privateKey;
    array<unsigned char, 20> m_addressRipemd160;
};

} /* namespace BitHack */

#endif /* CKEY_HPP_ */
