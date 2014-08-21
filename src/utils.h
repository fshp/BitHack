#ifndef UTILS_H_
#define UTILS_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <openssl/bio.h>

extern BIO    *out;

extern void initial_out();
extern void free_out();

#ifdef  __cplusplus
}
#endif

#endif /* UTILS_H_ */
