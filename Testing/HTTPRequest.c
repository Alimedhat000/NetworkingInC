#include "HTTPRequest.h"

#include <string.h>
#include <ctype.h>

#define MAX_REQUEST_SIZE 8192
// We will use strtok() to tokenize the request
// Please pray for my sanity 🤪

// *Note: strtok searches for the first instance of a char and returns the str before it
// also when doing multiple parsing you should pass null as strtok remembers the last inputted string

// lets say our request is :
// GET /favicon.ico HTTP/1.1 //* <========= This is the Request Line
// Host: localhost:5050
// Connection: keep-alive
// sec-ch-ua: "Chromium";v="128", "Not;A=Brand";v="24", "Opera";v="114"
// sec-ch-ua-mobile: ?0
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 OPR/114.0.0.0
// sec-ch-ua-platform: "Windows"
// Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
// Sec-Fetch-Site: same-origin
// Sec-Fetch-Mode: no-cors
// Sec-Fetch-Dest: image
// Referer: http://localhost:5050/
// Accept-Encoding: gzip, deflate, br, zstd
// Accept-Language: en-US,en;q=0.9

// ept-Encoding: gzip, deflate, br, zstd // * <========= This is the Body
// Accept-Language: en-US,en;q=0.9'

// ==========================================================
struct HTTPRequest HTTP_REQUEST_constructor(const char *request_buffer, size_t buffer_size)
{
    struct HTTPRequest httpRequest = {.method = 0, .URI = NULL, .version = 0, .headers = NULL, .body = NULL};

    if (buffer_size > MAX_REQUEST_SIZE)
    {
        // Request too large
        return httpRequest;
    }

    // Create a null-terminated copy of the buffer
    char *request = malloc(buffer_size + 1);
    if (!request)
    {
        return httpRequest;
    }
    memcpy(request, request_buffer, buffer_size);
    request[buffer_size] = '\0';

    // Find the end of the request line
    char *end_of_request_line = strstr(request, "\r\n");
    if (!end_of_request_line)
    {
        free(request);
        return httpRequest;
    }
    *end_of_request_line = '\0';

    // Parse the request line
    char *method = strtok(request, " ");
    char *URI = strtok(NULL, " ");
    char *version = strtok(NULL, " ");

    if (method && URI && version)
    {
        httpRequest.method = SetMethod(method);
        httpRequest.URI = strdup(URI);
        if (strncmp(version, "HTTP/", 5) == 0)
        {
            httpRequest.version = atof(version + 5);
        }
    }

    // Find the start of the body (if any)
    char *body_start = strstr(end_of_request_line + 2, "\r\n\r\n");
    if (body_start)
    {
        *body_start = '\0';
        body_start += 4;
        httpRequest.body = strdup(body_start);
    }

    // Everything between the request line and the body (or end) is headers
    httpRequest.headers = strdup(end_of_request_line + 2);

    free(request);
    return httpRequest;
}

enum HTTPMethods SetMethod(const char *method)
{
    if (strcmp(method, "GET") == 0)
        return GET;
    if (strcmp(method, "POST") == 0)
        return POST;
    if (strcmp(method, "PUT") == 0)
        return PUT;
    if (strcmp(method, "HEAD") == 0)
        return HEAD;
    if (strcmp(method, "PATCH") == 0)
        return PATCH;
    if (strcmp(method, "DELETE") == 0)
        return DELETE;
    if (strcmp(method, "CONNECT") == 0)
        return CONNECT;
    if (strcmp(method, "OPTIONS") == 0)
        return OPTIONS;
    if (strcmp(method, "TRACE") == 0)
        return TRACE;
    return -1; // Invalid method
}