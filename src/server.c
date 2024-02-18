#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "utils.h"
#include "server.h"

#define MAX_REQUEST_LEN 1024
#define DEBUG

#define FCLOSE(f)        \
    if (fclose(f) == -1) \
    perror("fclose (" #f ")")

#define FPUTS(s, f)        \
    if (fputs(s, f) == -1) \
    perror("fputs (" #f ")")

static int checkInitRequest(COCConfig *config, FILE *rx, FILE *tx)
{
    char line[MAX_REQUEST_LEN + 1];
    if (readLine(rx, line, MAX_REQUEST_LEN) == -1)
    {
        if (errno)
        {
            perror("Readline failed");
            FPUTS("FAIL SERVER", tx);
        }
        else
        {
            FPUTS("[FAIL] Request too long!\n", stderr);
            FPUTS("FAIL LENGTH", tx);
        }
        return -1;
    }
    char *command = strtok(line, " \n");
    if (!command || strcmp(command, "HELLO") != 0)
    {
        FPUTS("FAIL COMMAND", tx);
        FPUTS("[FAIL] No or invalid command found!\n", stderr);
        return -1;
    }
    char *name = strtok(NULL, " \n");
    if (!name || strlen(name) > 10)
    {
        FPUTS("FAIL NAME", tx);
        FPUTS("[FAIL] No or too long name found!\n", stderr);
        return -1;
    }
    if (fprintf(tx, "OK %s\n", config->name) < 0)
    {
        FPUTS("fprintf failed!\n", stderr);
    }
    if (fprintf(stdout, "[OK] New client: %s\n", name) < 0)
    {
        FPUTS("fprintf failed!\n", stderr);
    }
    return 0;
}

static void handleCommunication(COCConfig *config, FILE *rx, FILE *tx)
{

    if (checkInitRequest(config, rx, tx) == -1)
        return;
    fflush(tx);
}

static void handleConnection(COCConfig *config, int conn)
{
    FILE *rx = fdopen(conn, "r");
    if (!rx)
    {
        perror("fdopen (rx)");
        return;
    }
    int connDup = dup(conn);
    if (connDup == -1)
    {
        perror("dup");
        FCLOSE(rx);
        return;
    }
    FILE *tx = fdopen(connDup, "w");
    if (!tx)
    {
        perror("fdopen (tx)");
        FCLOSE(rx);
        return;
    }

    handleCommunication(config, rx, tx);

    fflush(tx);

    FCLOSE(rx);
    FCLOSE(tx);
}

void startServer(COCConfig *config)
{
    uint16_t port = htons(config->port);
    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (!sock)
    {
        die("Unable to create server socket");
    }

#ifdef DEBUG
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
#endif

    struct sockaddr_in6 addr = {
        .sin6_family = AF_INET6,
        .sin6_port = port,
        .sin6_addr = in6addr_any,
    };

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        die("Unable to bind socket");
    }
    if (listen(sock, SOMAXCONN) == -1)
    {
        die("Unable to listen on socket");
    }

    for (;;)
    {
        int conn = accept(sock, NULL, NULL);
        if (conn == -1)
        {
            continue;
        }
        handleConnection(config, conn);
    }
}