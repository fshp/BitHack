#include "CKeyFactory.hpp"
#include <openssl/obj_mac.h>

namespace BitHack {

const std::string CKeyFactory::base58_map =
        "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

CKeyFactory::CKeyFactory() {
    bn_ctx = BN_CTX_new();
    key = EC_KEY_new_by_curve_name(NID_secp256k1);

    //precompute for faster operation
    const EC_GROUP *group = EC_KEY_get0_group(key);
    EC_GROUP *precomputeGroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
    EC_GROUP_copy(precomputeGroup, group);

    EC_GROUP_precompute_mult(precomputeGroup, bn_ctx);

    EC_KEY_set_group(key, precomputeGroup);

    EC_GROUP_free(precomputeGroup);

    createKey();
}

CKeyFactory::~CKeyFactory() {
    EC_KEY_free(key);
    BN_CTX_free(bn_ctx);
}

void CKeyFactory::createKey() {
    EC_KEY_generate_key(key);
    BIGNUM *bn = BN_new();
    BN_one(bn);
    EC_KEY_set_private_key(key, bn);

    auto group = EC_KEY_get0_group(key);

    const EC_POINT *g = EC_GROUP_get0_generator(group);
    EC_POINT *newPubKey = EC_POINT_new(group);

    EC_POINT_mul(group, newPubKey, bn, NULL, NULL, bn_ctx);
    EC_KEY_set_public_key(key, newPubKey);

    BN_clear_free(bn);
}

CKey CKeyFactory::nextKey() {

    auto result = transform();

    const BIGNUM *privateKeyBN = EC_KEY_get0_private_key(key);
    BIGNUM *newPrivateKey = BN_new();
    BN_copy(newPrivateKey, privateKeyBN);
    BN_add_word(newPrivateKey, 1);

    EC_POINT *newPubKey = EC_POINT_new(EC_KEY_get0_group(key));
    const EC_POINT *g = EC_GROUP_get0_generator(EC_KEY_get0_group(key));
    const EC_POINT *pubKey = EC_KEY_get0_public_key(key);
    EC_POINT_add(EC_KEY_get0_group(key), newPubKey, g, pubKey, bn_ctx);

    //assert(EC_POINT_cmp(EC_KEY_get0_group(key), newPubKey, newPubKey2, bn_ctx) == 0);

    EC_KEY_set_private_key(key, newPrivateKey);
    EC_KEY_set_public_key(key, newPubKey);

    BN_free(newPrivateKey);
    EC_POINT_free(newPubKey);

    return result;
}

CKey CKeyFactory::transform() {
    privKey_t privKey;
    ripemd160_t addrUncompressed;
    ripemd160_t addrCompressed;
    {
        const BIGNUM *privateKeyBN = EC_KEY_get0_private_key(key);
        BN_bn2binpad(privateKeyBN, privKey.data(), privKey.size());

        std::array<unsigned char, 65> pubKey;
        EC_POINT_point2oct(EC_KEY_get0_group(key), EC_KEY_get0_public_key(key),
                           POINT_CONVERSION_UNCOMPRESSED, pubKey.data(), pubKey.size(), bn_ctx);

        std::array<unsigned char, 33> pubKeyCompressed;
        EC_POINT_point2oct(EC_KEY_get0_group(key), EC_KEY_get0_public_key(key),
                           POINT_CONVERSION_COMPRESSED, pubKeyCompressed.data(), pubKeyCompressed.size(), bn_ctx);

        std::array<unsigned char, 32> sha256_hash;
        SHA256_Init(&sha_ctx);
        SHA256_Update(&sha_ctx, pubKey.data(), pubKey.size());
        SHA256_Final(sha256_hash.data(), &sha_ctx);

        RIPEMD160_Init(&ripemd_ctx);
        RIPEMD160_Update(&ripemd_ctx, sha256_hash.data(), sha256_hash.size());
        RIPEMD160_Final(addrUncompressed.data(), &ripemd_ctx);

        SHA256_Init(&sha_ctx);
        SHA256_Update(&sha_ctx, pubKeyCompressed.data(), pubKeyCompressed.size());
        SHA256_Final(sha256_hash.data(), &sha_ctx);

        RIPEMD160_Init(&ripemd_ctx);
        RIPEMD160_Update(&ripemd_ctx, sha256_hash.data(), sha256_hash.size());
        RIPEMD160_Final(addrCompressed.data(), &ripemd_ctx);
    }
    return CKey(privKey, addrUncompressed, addrCompressed);
}

} /* namespace BitHack */
