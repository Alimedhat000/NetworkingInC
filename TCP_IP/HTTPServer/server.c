
#include "server.h"

struct Server sever_Constructor(sa_family_t domain, int service, int protocol,
                                in_addr_t interface, uint16_t port, int backlog, void (*launch)(struct Server *))
{
    struct Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port); // turn port from host to network
    server.address.sin_addr.s_addr = htonl(interface);

    server.Socket = socket(server.domain, server.service, server.protocol);
    check(server.Socket, "Failed to connect socket..");

    int opt = 1;
    check(setsockopt(server.Socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)),
          "Failed to set socket options");
    printf("Socket options set successfully.\n");

    check(bind(server.Socket, (SA *)&server.address, sizeof(server.address)),
          "Failed to Bind..");

    check(listen(server.Socket, server.backlog),
          "Listening Failed");

    printf("Server is listening for connections...\n");
    server.launch = launch;
    return server;
}

int check(int expressionStatus, char *msg)
{
    if (expressionStatus == SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }
    return expressionStatus;
}
