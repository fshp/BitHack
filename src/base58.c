#include "base58.h"
#include "utils.h"
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <string.h>

const char base58_map[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

char* key2addr(const EC_KEY *key) {
	unsigned char pub_key_oct[65];
	unsigned char sha_hash[32];
	unsigned char sha_hash_checksum1[32];
	unsigned char sha_hash_checksum2[32];
	unsigned char address_in_base256[1 + 20 + 4] = {0,};
	unsigned char address[48];
	SHA256_CTX    sha_ctx;
	RIPEMD160_CTX ripemd_ctx;
	BN_CTX       *bn_ctx;
	BIGNUM        bn_a, bn_b, bn_r, bn_base;
	BIGNUM       *bn_div, *bn_num;

	EC_POINT_point2oct( EC_KEY_get0_group(key),
						EC_KEY_get0_public_key(key),
						EC_KEY_get_conv_form(key),
						pub_key_oct,
						sizeof(pub_key_oct),
						NULL);

	SHA256_Init(&sha_ctx);
	SHA256_Update(&sha_ctx, pub_key_oct, sizeof(pub_key_oct));
	SHA256_Final(sha_hash, &sha_ctx);

	RIPEMD160_Init(&ripemd_ctx);
	RIPEMD160_Update(&ripemd_ctx, &sha_hash, sizeof(sha_hash));
	RIPEMD160_Final(address_in_base256 + 1, &ripemd_ctx);

	SHA256_Init(&sha_ctx);
	SHA256_Update(&sha_ctx, address_in_base256, sizeof(address_in_base256) - 4);
	SHA256_Final(sha_hash_checksum1, &sha_ctx);

	SHA256_Init(&sha_ctx);
	SHA256_Update(&sha_ctx, sha_hash_checksum1, sizeof(sha_hash_checksum1));
	SHA256_Final(sha_hash_checksum2, &sha_ctx);

	memcpy(address_in_base256 + 1 + 20, sha_hash_checksum2, 4                  );


	bn_ctx = BN_CTX_new();
	BN_init(&bn_a);
	BN_init(&bn_b);
	BN_init(&bn_r);
	BN_init(&bn_base);

	unsigned lending_zero;
	for(lending_zero = 0; lending_zero < sizeof(address_in_base256) && address_in_base256[lending_zero] == 0; ++lending_zero);

	bn_num = &bn_a;
	bn_div = &bn_b;

	BN_bin2bn(address_in_base256, sizeof(address_in_base256), bn_num);
	BN_set_word(&bn_base, 58);
	unsigned char *p = address + sizeof(address);
	while(!BN_is_zero(bn_num)) {
		BN_div(bn_div, &bn_r, bn_num, &bn_base, bn_ctx);

		*--p = base58_map[BN_get_word(&bn_r)];

		BIGNUM *bn_tmp = bn_num;
		bn_num = bn_div;
		bn_div = bn_tmp;
	};

	while(lending_zero--){
		*--p = base58_map[0];
	};

	BN_clear_free(&bn_a);
	BN_clear_free(&bn_b);
	BN_clear_free(&bn_r);
	BN_clear_free(&bn_base);
	BN_CTX_free(bn_ctx);

	const BIGNUM *priv_key = EC_KEY_get0_private_key(key);
	char *bn_hex = BN_bn2hex(priv_key);
	BIO_printf(out, "Private key: \t  %s\n", bn_hex);
	OPENSSL_free(bn_hex);


	BIO_printf(out, "RIPEMD160 of public key: \t  ");
	for(unsigned i = 1; i < sizeof(address_in_base256) - 4; ++i)
		BIO_printf(out, "%02x", address_in_base256[i]);
	BIO_printf(out, "\n");

	BIO_printf(out, "Addr in Base256: \t\t");
	for(unsigned i = 0; i < sizeof(address_in_base256); ++i)
		BIO_printf(out, "%02x", address_in_base256[i]);
	BIO_printf(out, "\n");

	BIO_printf(out, "Addr in Base58: \t\t");
	for(; p < address + sizeof(address); ++p)
		BIO_printf(out, "%c", *p);
	BIO_printf(out, "\n");

	return 0;
}
