#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "server.h"

#define MAX_REQUEST_LEN 1024
#define DEBUG

#define FCLOSE(f)        \
    if (fclose(f) == -1) \
    perror("fclose (" #f ")")

static int checkInitRequest(FILE *rx, FILE *tx)
{
    char *line = readLine(rx, MAX_REQUEST_LEN);
    if (!line)
    {
        fprintf(stderr, "Request too long!\n");
        fputs("FAIL LENGTH\n", tx);
        return -1;
    }
    if (strncmp(line, "HELLO ", 6))
    {
        fprintf(stderr, "Invalid request!\n");
        free(line);
        fputs("FAIL HELLO\n", tx);
        return -1;
    }
    char *curr = line + 6;
    while (*curr == ' ')
    {
        curr++;
    };
    char name[11];
    int i = 0;
    while (i < 10 && isalpha(*(curr)))
    {
        name[i++] = *(curr++);
    };
    name[i] = 0;
    while (*(curr) == ' ')
    {
        curr++;
    };
    if (*curr != '\n')
    {
        fprintf(stderr, "Invalid request!\n");
        free(line);
        fputs("FAIL NAME", tx);
        return -1;
    }
    free(line);
    fprintf(tx, "OK %s\n", name);
    return 0;
}

static void handleCommunication(FILE *rx, FILE *tx)
{

    if (checkInitRequest(rx, tx) == -1)
        return;
    fflush(tx);
}

static void handleConnection(int conn)
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

    handleCommunication(rx, tx);

    fflush(tx);

    FCLOSE(rx);
    FCLOSE(tx);
}

void startServer(uint16_t port)
{
    port = htons(port);
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
        handleConnection(conn);
    }
}