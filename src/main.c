#include "openssl/ec.h"
#include "openssl/obj_mac.h"
#include "openssl/bio.h"

int main() {

    BIO    *out = NULL;
    EC_KEY *key = NULL;

    out = BIO_new_fp(stdout, BIO_FLAGS_WRITE);

    key = EC_KEY_new_by_curve_name(NID_secp256k1);

    EC_KEY_generate_key(key);
    EC_KEY_print(out, key, 0);

    EC_KEY_free(key);
    BIO_free(out);
    return (0);
}
