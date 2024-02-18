#pragma once

#include <stdio.h>

#define STYLE(c) printf("\x1b[" #c "m")
#define RESET() STYLE(0)

#define ERASE(c) printf("\x1b[" #c "J")
#define CLEAR() ERASE(2)

#define HOME() printf("\x1b[H")