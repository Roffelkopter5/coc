#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#include "client.h"

static int safe_getaddrinfo(const char *host, const char *port, const struct addrinfo *hints, struct addrinfo **res)
{
    int err = getaddrinfo(host, port, hints, res);
    if (err)
    {
        if (err == EAI_SYSTEM)
        {
            perror("getaddrinfo");
        }
        else
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        }
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void startClient(COCConfig *config)
{
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_ADDRCONFIG};

    struct addrinfo *head, *current;

    if (safe_getaddrinfo("localhost", "6969", &hints, &head))
    {
        return;
    }

    if (head == NULL)
    {
        fprintf(stderr, "No host found\n");
        return;
    }

    int sock;

    for (current = head; current; current = current->ai_next)
    {
        sock = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
        if (sock == -1)
        {
            perror("socket");
            freeaddrinfo(head);
            return;
        }
        if (connect(sock, current->ai_addr, current->ai_addrlen) == 0)
        {
            break;
        }
        close(sock);
    }

    freeaddrinfo(head);

    if (current == NULL)
    {
        fprintf(stderr, "Unable to connect to host\n");
        return;
    }
}