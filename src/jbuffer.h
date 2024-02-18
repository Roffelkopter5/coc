#include <stdlib.h>

typedef struct BNDBUF BNDBUF;

BNDBUF *bbCreate(size_t size);

void bbDestroy(BNDBUF *bb);

void bbPut(BNDBUF *bb, int value);

int bbGet(BNDBUF *bb);
