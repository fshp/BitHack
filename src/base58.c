#include "base58.h"
#include "utils.h"
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/ec.h>
#include <string.h>

char* key2addr(const EC_KEY *key) {
	unsigned char pub_key_oct[65];
	unsigned char sha_hash[32];
	unsigned char ripemd_hash[20];
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
	RIPEMD160_Final(ripemd_hash, &ripemd_ctx);


	BIO_printf(out, "RIPEMD160 of public key: ");
	for(unsigned i = 0; i < sizeof(ripemd_hash); ++i)
		BIO_printf(out, "%02x", ripemd_hash[i]);
	BIO_printf(out, "\n");

	return 0;
}
