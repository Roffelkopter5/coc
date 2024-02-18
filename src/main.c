#include <stdio.h>

#include "server.h"
#include "config.h"

int main()
{
    COCConfig *config = configInit();
    printf("Port: %d\n", config->port);
    startServer(config->port);
}