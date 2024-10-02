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

typedef struct sockaddr_in SA_IN; // redefining the names for easier
typedef struct sockaddr SA;

#define SERVERPORT 5555
#define SERVERBACKLOG 10
#define BUFFERSIZE 100
#define SOCKETERROR (-1)

int check(int expressionStatus, char *msg);
void *handle_connection(void *Client_Socket_P);

int main(int argc, char const *argv[])
{
    int Server_socket, Client_Socket;
    SA_IN Server_addr, Client_addr;

    printf("Starting server...\n"); // Debug statement

    check(Server_socket = socket(AF_INET, SOCK_STREAM, 0), // IPv4, and Stream for TCP/IP.
          "Failed To create Socket");
    printf("Socket created successfully.\n");

    int opt = 1;
    check(setsockopt(Server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)),
          "Failed to set socket options");
    printf("Socket options set successfully.\n");

    Server_addr.sin_family = AF_INET;
    Server_addr.sin_port = htons(SERVERPORT); // htons converts from host byte order to network byte order
    Server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    check(bind(Server_socket, (SA *)&Server_addr, sizeof(Server_addr)),
          "Binding Failed");
    printf("Socket bound to port %d successfully.\n", SERVERPORT);

    check(listen(Server_socket, SERVERBACKLOG),
          "Listening Failed");
    printf("Server is listening for connections...\n");

    while (true)
    {
        int ADDRSIZE = sizeof(SA_IN);
        printf("Waiting for connections...\n"); // Debug statement
        check(Client_Socket = accept(Server_socket, (SA *)&Client_addr, (socklen_t *)&ADDRSIZE),
              "Accept Failed");
        printf("Connection Established!\n");

        // Handle the connection directly
        handle_connection((void *)(intptr_t)Client_Socket); // Cast to void pointer
    }

    return 0;
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

void *handle_connection(void *Client_Socket_p)
{
    int Client_Socket = (intptr_t)Client_Socket_p; // Cast back to int
    char buffer[BUFFERSIZE];
    ssize_t bytes_read;

    while (true) // Loop to keep the connection alive
    {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer

        // Read from the client
        bytes_read = read(Client_Socket, buffer, BUFFERSIZE - 1);
        check(bytes_read, "Receive Error");

        // Check for disconnection
        if (bytes_read == 0)
        {
            printf("Client disconnected.\n");
            break; // Exit the loop
        }

        // Null-terminate the received data
        buffer[bytes_read] = '\0';

        // Check for exit condition
        if (strcmp(buffer, "q\n") == 0) // Check if the received message is "q"
        {
            printf("Client requested to quit.\n");
            break; // Exit the loop
        }

        // Print the request and send the echo back
        printf("Request: %s\n", buffer);
        fflush(stdout);

        // Echo the same message back to the client
        ssize_t bytes_written = write(Client_Socket, buffer, bytes_read);
        check(bytes_written, "Send Error");
    }

    close(Client_Socket); // Close the client socket
    return NULL;
}