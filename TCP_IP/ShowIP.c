/*
** showip.c -- show IP addresses for a host given on the command line
*/

#include <stdio.h>      // For standard input/output functions
#include <stdlib.h>     // For memory allocation, etc.
#include <string.h>     // For memory manipulation
#include <sys/types.h>  // For system types like `socklen_t`
#include <sys/socket.h> // For socket definitions
#include <netdb.h>      // For addrinfo, getaddrinfo
#include <arpa/inet.h>  // For inet_ntop
#include <netinet/in.h> // For sockaddr_in and sockaddr_in6

int main(int argc, char *argv[])
{
    // struct addrinfo {
    //     int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    //     int              ai_family;    // You can force it to use IPv4 or IPv6 in the ai_family field, or leave it as AF_UNSPEC to use whatever.( AF_INET, AF_INET6, AF_UNSPEC )
    //     int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    //     int              ai_protocol;  // use 0 for "any"
    //     size_t           ai_addrlen;   // size of ai_addr in bytes
    //     struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    //     char            *ai_canonname; // full canonical hostname
    //     struct addrinfo *ai_next;      // linked list, next node
    // };
    struct addrinfo hints, *result, *pointer;
    // hints: Specifies the criteria for address conversion.
    // result: pointer to a linked list of addrinfo returned by getaddrinfo().
    // pointer: Pointer to traverse the linked list. 
    int status; // a flag to check if getaddrinfo was successful or not.
    char ipstr[INET6_ADDRSTRLEN];    // a string to hold the IP address.
                                    // INET6_ADDRSTRLEN: Length of IPV6 address.

    if (argc != 2)  // checks if the commandLine args are exactly two 
                    // One is the file itself and the second is the given address.  
    {
        fprintf(stderr, "\033[0;31mError! Please Enter One Address only\n"); //\033[0;31m Turns the text to read lol :3
        printf("\033[0m"); // Resets the color again
        return 1;
    }

    memset(&hints, 0, sizeof(hints));   // Initialize all Hints members to zero
    hints.ai_family = AF_UNSPEC;        // Dont Care, You can put AF_INET(IPv4) or AF_INET6(IPv6) to force version
    hints.ai_socktype = SOCK_STREAM;    // Choosing Stream_Socket(TCP) as the socktype

    if ((status = getaddrinfo(argv[1], NULL, &hints, &result)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", argv[1]);

    // Loops through all the linked list nodes to print all the addresses of the hostname
    for (pointer = result; pointer != NULL; pointer = pointer->ai_next)
    {
        void *addr;     // A pointer that will point to the address data (IPv4 or IPv6).
        char *ipver;    // A string to indicate the version of IP being used.

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (pointer->ai_family == AF_INET)
        { // IPv4
            // (IPv4 only)
            // struct sockaddr_in {
            //     short int          sin_family;  // Address family, AF_INET
            //     unsigned short int sin_port;    // Port number
            //     struct in_addr     sin_addr;    // Internet address--->uint32_t s_addr; // that's a 32-bit int (4 bytes)
            //     unsigned char      sin_zero[8]; // Same size as struct sockaddr as padding
            // };
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)pointer->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        { // IPv6
            // (IPv6 only)
            // struct sockaddr_in6 {
            //     u_int16_t       sin6_family;   // address family, AF_INET6
            //     u_int16_t       sin6_port;     // port number, Network Byte Order
            //     u_int32_t       sin6_flowinfo; // IPv6 flow information
            //     struct in6_addr sin6_addr;     // IPv6 address ---->unsigned char   s6_addr[16];   // IPv6 address(128 bits)
            //     u_int32_t       sin6_scope_id; // Scope ID
            // };
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)pointer->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // convert the IP to a string and print it:
        inet_ntop(pointer->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(result); // free the linked list

    return 0;
}