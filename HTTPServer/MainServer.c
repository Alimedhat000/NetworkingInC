#include "server.h"

void launch(struct Server *server);

void launch(struct Server *server)
{
    char buffer[BUFFERSIZE];
    char *hello = "HTTP/1.1 200 OK\n"
                  "Date: Mon, 27 Jul 2009 12:28:53 GMT\n"
                  "Server: Apache/2.2.14 (Win32)\n"
                  "Content-Length: 48\n"
                  "Content-Type: text/html\n"
                  "Connection: Closed\n"
                  "\n" // Extra newline is critical!
                  "<html><body><h1>Hello, World!</h1></body></html>";
    int Client_sock;
    socklen_t address_length = sizeof(server->address);

    printf("======== Wait For Connection =========\n");
    while (1)
    {
        Client_sock = accept(server->Socket, (SA *)&server->address,
                             &address_length); // Pass the address of socklen_t variable

        check(Client_sock, "Failed To Accept");
        read(Client_sock, buffer, BUFFERSIZE - 1);
        printf("Received: %s \n", buffer); // print buffer for now!
        send(Client_sock, hello, strlen(hello), 0);
        close(Client_sock);
    }
}

int main()
{
    struct Server server = sever_Constructor(AF_INET,
                                             SOCK_STREAM, 0, INADDR_ANY, 5050, 10, launch);

    server.launch(&server);

    return 0;
}
