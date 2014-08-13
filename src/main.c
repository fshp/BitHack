#include "openssl/ec.h"
#include "openssl/obj_mac.h"
#include "openssl/bio.h"

int main() {

    BIO    *out = NULL;
    EC_KEY *key = NULL;
    BN_CTX *ctx = NULL;
    unsigned char buf[1024];


    out = BIO_new_fp(stdout, BIO_FLAGS_WRITE);
    ctx = BN_CTX_new();

    key = EC_KEY_new_by_curve_name(NID_secp256k1);

    EC_KEY_generate_key(key);
    EC_KEY_print(out, key, 0);

    const EC_POINT *pub = EC_KEY_get0_public_key(key);
    char *pub_hex = EC_POINT_point2hex( EC_KEY_get0_group(key), pub,
                                        EC_KEY_get_conv_form(key), ctx);

    size_t len = EC_POINT_point2oct(EC_KEY_get0_group(key),
    								pub,
									EC_KEY_get_conv_form(key),
									buf,
									sizeof(buf),
									ctx);

    if(len != 0 && len < sizeof(buf)) {
    	buf[len] = 0x00;
    	//BIO_printf(out, "Bin Pub Address: %s\n", buf);
    	unsigned i = 0;
    	for(; i < len; ++i) {
    		char map[] = "0123456789abcdef";
    		char tmp[2] = "0";
    		tmp[0] = map[buf[i] / 16];
    		BIO_printf(out, tmp);
    		tmp[0] = map[buf[i] % 16];
    		BIO_printf(out, tmp);
    		if(i != len - 1)
    			BIO_printf(out, ":");
    	}
    	BIO_printf(out, "\n");

    }

    BIO_printf(out, "%s\n", pub_hex);

    OPENSSL_free(pub_hex);
    BN_CTX_free(ctx);
    EC_KEY_free(key);
    BIO_free(out);
    return (0);
}
