#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

void *safe_malloc(size_t size);

char *readLine(FILE *file, const size_t max_len);

noreturn void die(const char *msg);