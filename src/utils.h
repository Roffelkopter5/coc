#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

void *safe_malloc(size_t size);

int readLine(FILE *file, char *buffer, const size_t max_len);

noreturn void die(const char *msg);