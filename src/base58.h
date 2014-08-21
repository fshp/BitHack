#ifndef BASE58_H_
#define BASE58_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <openssl/ec.h>

extern char* key2addr(const EC_KEY *key);

#ifdef  __cplusplus
}
#endif

#endif /* BASE58_H_ */
