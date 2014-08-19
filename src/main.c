#include "openssl/ec.h"
#include "openssl/obj_mac.h"
#include "base58.h"
#include "utils.h"

int main() {
    EC_KEY *key = NULL;

    initial_out();

    key = EC_KEY_new_by_curve_name(NID_secp256k1);

    EC_KEY_generate_key(key);
    EC_KEY_print(out, key, 0);
    key2addr(key);

    EC_KEY_free(key);
    free_out();

    return (0);
}
