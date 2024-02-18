#include <string.h>
#include <errno.h>

#include "utils.h"

int readLine(FILE *file, char *buffer, const size_t max_len)
{
    if (feof(file) || !buffer)
    {
        return -1;
    }
    buffer[max_len] = 0;
    size_t i = 0;
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        if (i < max_len)
        {
            buffer[i] = (char)c;
        }
        i++;
        if (c == '\n')
        {
            break;
        }
    }
    if (ferror(file))
    {
        return -1;
    }
    if (i > max_len)
    {
        return -1;
    }
    buffer[i] = 0;
    return 0;
}

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        perror("Cannot allocate memory");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

noreturn void die(const char *msg)
{
    if (errno)
    {
        perror(msg);
    }
    else
    {
        fprintf(stderr, "%s\n", msg);
    }
    exit(EXIT_FAILURE);
}
