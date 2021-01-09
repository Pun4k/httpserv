#include "../hashtable/hashtab.h"

#include "../net/net.h"
#include "http.h"

typedef struct HTTP {
    char *host;
    i32 len;
    i32 cap;
    void(**funcs)(int, HTTPreq*);
    HashTab *tab;
} HTTP;