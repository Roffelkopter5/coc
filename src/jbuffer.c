#include <stdlib.h>
#include <stdatomic.h>
#include <limits.h>

#include "jbuffer.h"
#include "sem.h"

struct BNDBUF
{
    SEM *empty;
    SEM *full;
    _Atomic size_t nextout;
    size_t nextin;
    size_t capacity;
    int *data;
};

BNDBUF *bbCreate(size_t size)
{
    if (size > INT_MAX)
    {
        return NULL;
    }
    BNDBUF *buf = malloc(sizeof(BNDBUF));
    if (!buf)
    {
        return NULL;
    }
    buf->empty = NULL;
    buf->empty = semCreate(size);
    if (!buf->empty)
    {
        bbDestroy(buf);
        return NULL;
    }
    buf->full = NULL;
    buf->full = semCreate(0);
    if (!buf->full)
    {
        bbDestroy(buf);
        return NULL;
    }
    buf->nextin = 0;
    atomic_init(&buf->nextout, 0);
    buf->capacity = size;
    buf->data = NULL;
    buf->data = malloc(sizeof(int) * size);
    if (!buf->data)
    {
        bbDestroy(buf);
        return NULL;
    }
    return buf;
}

void bbDestroy(BNDBUF *bb)
{
    if (!bb)
        return;
    semDestroy(bb->empty);
    semDestroy(bb->full);
    free(bb->data);
    free(bb);
}

void bbPut(BNDBUF *bb, int value)
{
    P(bb->empty);
    bb->data[bb->nextin] = value;
    bb->nextin = (bb->nextin + 1) % bb->capacity;
    V(bb->full);
}

int bbGet(BNDBUF *bb)
{
    P(bb->full);
    size_t nextout;
    int value;
    do
    {
        nextout = atomic_load(&bb->nextout);
        value = bb->data[nextout];
    } while (!atomic_compare_exchange_strong(&bb->nextout, &nextout, (nextout + 1) % bb->capacity));
    V(bb->empty);
    return value;
}
