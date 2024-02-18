#include <string.h>
#include <errno.h>

#include "utils.h"

char *readLine(FILE *file, const size_t max_len)
{
    if (feof(file))
    {
        return NULL;
    }
    char buffer[max_len];
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
        return NULL;
    }
    if (i > max_len)
    {
        return NULL;
    }
    buffer[i] = 0;
    char *line = (char *)safe_malloc(i);
    strcpy(line, buffer);
    return line;
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
