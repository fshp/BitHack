#ifndef CKEYFACTORY_HPP_
#define CKEYFACTORY_HPP_

#include <openssl/ec.h>
#include <openssl/ossl_typ.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <string>
#include "CKey.hpp"

namespace BitHack {

using namespace std;

class CKeyFactory {
public:
    CKeyFactory();
    virtual ~CKeyFactory();
    CKey createKey();
    CKey nextKey();

protected:
    BN_CTX *bn_ctx;
    RIPEMD160_CTX ripemd_ctx;
    SHA256_CTX sha_ctx;
    EC_KEY *key;
    static const string base58_map;
    CKey transform();

};

} /* namespace BitHack */

#endif /* CKEYFACTORY_HPP_ */
