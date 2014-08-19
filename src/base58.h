#ifndef BASE58_H_
#define BASE58_H_

#include <openssl/ec.h>

extern char* key2addr(const EC_KEY *key);

#endif /* BASE58_H_ */
