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
    int Client_Socket;
    SA_IN Server_addr;
    char buffer[BUFFERSIZE];
    char message[BUFFERSIZE];
    ssize_t bytes_read;

    // Create socket
    check(Client_Socket = socket(AF_INET, SOCK_STREAM, 0), "Failed To create Socket");

    // Server address setup
    Server_addr.sin_family = AF_INET;
    Server_addr.sin_port = htons(SERVERPORT); // htons converts from host byte order to network byte order
    Server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    check(connect(Client_Socket, (const SA *)&Server_addr, sizeof(Server_addr)), "Failed To Connect");

    while (true)
    {

        int status = 0;
        printf("Enter the message to be echoed (type 'q' to quit): ");
        fgets(message, BUFFERSIZE, stdin);

        // Check for exit condition
        if (strcmp(message, "q\n") == 0)
        {
            status = 1;
        }

        // Send message to server
        check(write(Client_Socket, message, strlen(message)), "Write failed");

        // Read the echoed message from server
        bytes_read = read(Client_Socket, buffer, BUFFERSIZE - 1);
        check(bytes_read, "Read Failed");

        // Null-terminate the buffer
        buffer[bytes_read] = '\0';
        printf("Echoed Message from Server: %s\n", buffer);
        
        if (status)
        {
            close(Client_Socket);
            exit(0);
        }
    }
    return 0;
}

// Function to check errors in socket-related calls
int check(int expressionStatus, char *msg)
{
    if (expressionStatus == SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }
    return expressionStatus;
}
