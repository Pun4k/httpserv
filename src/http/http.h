#ifndef HTTP_H
#define HTTP_H

#include <stdint.h>
#include <stdlib.h>

#define u8 uint8_t
#define i8 int8_t
#define i32 int32_t

typedef struct HTTPreq
{
    char method[16];
    char path[2048];
    char proto[16];
    u8 state;
    size_t index;
} HTTPreq;

typedef struct HTTP HTTP;

extern HTTP* new_http(char* address);
extern void free_http(HTTP *http);

extern void handle_http(HTTP *http, char *path, void(*)(int, HTTPreq*));
extern i8 listen_http(HTTP *http);

extern void parse_html_http(int conn, char* filename);

#endif //!HTTP_H