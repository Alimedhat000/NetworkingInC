#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <stdio.h>
#include <stdlib.h>
#include "./HashTables/HashTables.h"

enum HTTPMethods
{
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE
};

struct HTTPRequest
{
    enum HTTPMethods method;
    char *URI;
    float version;
    char *headers_string;
    char *body;
    hash_table *header_value;
};

struct HTTPRequest HTTP_REQUEST_constructor(const char *request_buffer, size_t buffer_size);
enum HTTPMethods SetMethod(const char *method);
char *trim(char *str);
void HTTPRequest_destructor(struct HTTPRequest *request);
void ParsingTester();

#endif // !HTTP_REQUEST
