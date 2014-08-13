#include "openssl/ec.h"
#include "openssl/obj_mac.h"
#include "openssl/bio.h"

int main() {

    BIO    *out = NULL;
    EC_KEY *key = NULL;
    BN_CTX *ctx = NULL;

    out = BIO_new_fp(stdout, BIO_FLAGS_WRITE);
    ctx = BN_CTX_new();

    key = EC_KEY_new_by_curve_name(NID_secp256k1);

    EC_KEY_generate_key(key);
    EC_KEY_print(out, key, 0);

    const EC_POINT *pub = EC_KEY_get0_public_key(key);
    char *pub_hex = EC_POINT_point2hex( EC_KEY_get0_group(key), pub,
                                        EC_KEY_get_conv_form(key), ctx);

    BIO_printf(out, "%s\n", pub_hex);

    OPENSSL_free(pub_hex);
    BN_CTX_free(ctx);
    EC_KEY_free(key);
    BIO_free(out);
    return (0);
}
