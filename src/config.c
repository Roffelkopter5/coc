#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "utils.h"
#include "list.h"
#include "memory.h"

#define MAX_LINE_LENGTH 10

static void parseLine(char *line, COCConfig *config)
{
    if (strncmp(line, "name=", 5) == 0)
    {
        config->name = line + 5;
    }
    else
    {
        fprintf(stderr, "Invalid config entry! Skipping '%s'\n", line);
    }
}

static void loadConfigFromFile(FILE *file, COCConfig *config)
{
    char *line;
    while ((line = readLine(file, MAX_LINE_LENGTH)))
    {
        parseLine(line, config);
    }
}

static void loadDefaults(COCConfig *config)
{
    if (!config->name)
    {
        config->name = "DefaultName";
    }
    if (!config->port)
    {
        config->port = "6969";
    }
}

COCConfig *configInit(void)
{
    COCConfig *config = (COCConfig *)safe_malloc(sizeof(COCConfig));
    FILE *file = fopen(".coc-config", "r");
    if (file)
    {
        loadConfigFromFile(file, config);
    }
    loadDefaults(config);
    return config;
}