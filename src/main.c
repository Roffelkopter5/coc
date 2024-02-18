#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "config.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        return EXIT_FAILURE;
    }
    COCConfig *config = configInit();
    if (!config)
        return EXIT_FAILURE;
    if (*argv[1] == 's')
    {
        startServer(config);
    }
    else if (*argv[1] == 'c')
    {
        startClient(config);
    }
    else
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
