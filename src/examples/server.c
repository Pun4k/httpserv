#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "../net/net.h"

#define BUFF_SIZE 256

int main(void) {
    int listener = listen_net("0.0.0.0:7878");
    if(listener < 0) {
        fprintf(stderr, "%d\n", listener);
        return 1;
    }

    printf("listening.. \n");
    char buffer[BUFF_SIZE];

    while(1) {
        int conn = accept_net(listener);
        if(conn < 0) {
            fprintf(stderr, "ERR: accept\n");
            return 2;
        }
        while(1) {
            int lenght = recv_net(conn, buffer, BUFF_SIZE);
            if(lenght <= 0) {
                break;
            }
            for(char* p = buffer; *p != '\0'; ++p) {
                *p = toupper(*p);
            }
            send_net(conn, buffer, BUFF_SIZE);
        }
        close_net(conn);
    }
    return 0;
}