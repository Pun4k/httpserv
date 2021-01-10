#ifndef HTTPSERV_NET
#define HTTPSERV_NET

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define u8 uint8_t
#define u32 uint32_t
#define i8 int8_t
#define i32 int32_t

typedef enum Error_t {
    WINSOCK_ERR = -1,
    SOCKET_ERR = -2,
    SETOPT_ERR = -3,
    PARSE_ERR = -4,
    BIND_ERR = -5,
    LISTEN_ERR = -6,
    CONNECT_ERR = -7
}Error;

static i8 parse_address(const char* address, char* ipv4, char* port);

extern i32 listen_net(char* address);
extern i32 accept_net(int listener);

extern i32 connect_net(char* address);
extern i32 close_net(int conn);

extern i32 send_net(int conn, char* buff, size_t size);
extern i32 recv_net(int conn, char* buff, size_t size);

#endif //HTTPSERV_NET_H
