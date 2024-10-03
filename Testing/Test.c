#include <stdio.h>
#include <string.h>
#include "HTTPRequest.h"

// Assume the HTTPRequest struct and HTTP_REQUEST_constructor function are defined here or included

int main()
{
    const char test_request[] =
        "GET /index.html HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
        "Accept-Language: en-US,en;q=0.5\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Connection: keep-alive\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "Cache-Control: max-age=0\r\n\r\n"
        "This is the body of the request";

    size_t request_length = strlen(test_request);

    struct HTTPRequest request = HTTP_REQUEST_constructor(test_request, request_length);

    // Print parsed information
    printf("Method: %d\n", request.method);
    printf("URI: %s\n", request.URI);
    printf("Version: %.1f\n", request.version);
    printf("Headers:\n%s\n", request.headers);
    printf("Body:\n%s\n", request.body ? request.body : "No body");

    // // Don't forget to free the allocated memory
    // HTTPRequest_destructor(&request);

    return 0;
}