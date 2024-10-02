#ifndef MY_HTTP_SERVER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

#define SOCKETERROR (-1)
#define BUFFERSIZE 4096

struct Server
{
    sa_family_t domain;
    int service;
    int protocol;
    in_addr_t interface;
    int port;
    int backlog;

    SA_IN address;

    int Socket;

    void (*launch)(struct Server *); // A function pointer to start the server
};

struct Server sever_Constructor(sa_family_t domain, int service, int protocol,
                                in_addr_t interface, uint16_t port, int backlog, void (*launch)(struct Server *));
int check(int expressionStatus, char *msg);
#endif // MY_HTTP_SERVER