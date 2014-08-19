#ifndef UTILS_H_
#define UTILS_H_

#include <openssl/bio.h>

extern BIO    *out;

extern void initial_out();
extern void free_out();

#endif /* UTILS_H_ */
