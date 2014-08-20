#include "base58.h"
#include "utils.h"
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/ec.h>
#include <string.h>

char* key2addr(const EC_KEY *key) {
	unsigned char pub_key_oct[65];
	unsigned char sha_hash[32];
	unsigned char sha_hash_checksum1[32];
	unsigned char sha_hash_checksum2[32];
	unsigned char address_in_base256[1 + 20 + 4] = {0,};
	SHA256_CTX    sha_ctx;
	RIPEMD160_CTX ripemd_ctx;

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

	BIO_printf(out, "RIPEMD160 of public key: \t  ");
	for(unsigned i = 1; i < sizeof(address_in_base256) - 4; ++i)
		BIO_printf(out, "%02x", address_in_base256[i]);
	BIO_printf(out, "\n");

	BIO_printf(out, "Addr in Base256: \t\t");
	for(unsigned i = 0; i < sizeof(address_in_base256); ++i)
		BIO_printf(out, "%02x", address_in_base256[i]);
	BIO_printf(out, "\n");

	return 0;
}
