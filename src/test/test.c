#include "stdio.h"
#include "stddef.h"
#include "string.h"

#include "../net/net.h"

int test_one() {
    int listener = listen_net("0.0.0.0:7878");
    if (listener < 0) {
        fprintf(stderr, "%d\n", listener);
    }

    printf("Server is listening...\n");

    close_net(listener);
    printf("Server closed connection...\n");
    return 0;
}