#include <stdio.h>
#include <stdint.h>

#include "../net/net.h"

#define BUFF_SIZE 256
#define INDEX(ptr, init) (ptr - init)

void input(char* buffer, int size);

int main(void) {
    int conn = connect_net("127.0.0.1:7878");
    if(conn < 0) {
        fprintf(stderr, "%d\n", conn);
        return 1;
    }

    char buffer[BUFF_SIZE];
    input(buffer, BUFF_SIZE);

    send_net(conn, buffer, BUFF_SIZE);
    recv_net(conn, buffer, BUFF_SIZE);

    printf("%s\n", buffer);
    close_net(conn);

    return 0;
}

void input(char* buffer, int size) {
    if(size == 0) {
        return;
    }
    char* ptr = buffer;
    while((*ptr = getchar()) != '\n' && INDEX(ptr,buffer) < (size-1)) {
        ++ptr;
    }
    *ptr = '\0';
}