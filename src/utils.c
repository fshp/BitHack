#include "utils.h"

BIO *out = NULL;

void initial_out() {
	out = BIO_new_fp(stdout, BIO_FLAGS_WRITE);
}

void free_out() {
	BIO_free(out);
}
