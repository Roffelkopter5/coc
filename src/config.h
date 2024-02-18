#pragma once

typedef struct COCConfig
{
    char *name;
    uint16_t port;
} COCConfig;

COCConfig *configInit(void);