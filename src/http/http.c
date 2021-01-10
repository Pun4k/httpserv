#include "http.h"

typedef struct HTTP {
    char *host; //addres
    i32 len;
    i32 cap;
    void(**funcs)(int, HTTPreq*);
    HashTab *tab;
} HTTP;

/*************/
/*STATIC FUNC*/
/*************/

static void page404_http_(int conn) {
    char *header = "HTTP/1.1 404 Not Found\n\nnot found";
    size_t headsize = strlen(header);
    send_net(conn, header, headsize);
}

static HTTPreq new_req_(void) {
    return (HTTPreq){
        .method = {0},
        .path = {0},
        .proto = {0},
        .state = 0,
        .index = 0
    };
}

static void null_req_(HTTPreq *req) {
    req->state += 1;
    req->index = 0;
}

static void parse_req_(HTTPreq *req, char *buffer, size_t size) {
    printf("%s\n", buffer);
    for(size_t i = 0; i < size; ++i) {
        switch (req->state) {
        case 0:
            if(buffer[i] == ' ' || req->index == METHOD_SIZE-1) {
                req->method[req->index] = '\0';
                null_req_(req);
                continue;
            }
            req->method[req->index] = buffer[i];
            break;
        case 1:
            if(buffer[i] == ' ' || req->index == PATH_SIZE-1) {
                req->path[req->index] = '\0';
                null_req_(req);
                continue;
            }
            req->path[req->index] = buffer[i];
            break;
        case 2:
            if(buffer[i] == '\n' || req->index == PROTO_SIZE-1) {
                req->proto[req->index] = '\0';
                null_req_(req);
                continue;
            }
            req->proto[req->index] = buffer[i];
            break;
        default:
            return;
        }
        req->index += 1;
    }
}

static i32 switch_http_(HTTP *http, int conn, HTTPreq *req) {
    if(!in_hashtab(http->tab, string(req->path))) {
        char buffer[PATH_SIZE];
        memcpy(buffer, req->path, PATH_SIZE);
        i32 index = strlen(req->path);
        if(index == 0) {
            page404_http_(conn);
            return 1;
        }
        index -= 1;
//        buffer[index] = '\0';
        for(; index > 0 && buffer[index] != '/'; --index) {
            buffer[index] = '\0';
        }
        if(!in_hashtab(http->tab, string(req->path))) {
            page404_http_(conn);
            return 2;
        }
        index = get_hashtab(http->tab, string(buffer)).decimal;
        http->funcs[index](conn, req);
        return 0;
    }
    i32 index = get_hashtab(http->tab, string(req->path)).decimal;
    http->funcs[index](conn, req);
    return 0;
}

/*************/
/*EXTERN FUNC*/
/*************/

extern HTTP* new_http(char* address) {
    HTTP* http = (HTTP*)malloc(sizeof(HTTP));
    http->cap = 1000;
    http->len = 0;
    http->host = (char*)malloc(sizeof(char) * strlen(address) + 1);
    strcpy(http->host, address);
    http->tab = new_hashtab(http->cap, STRING_ELEM, DECIMAL_ELEM);
    http->funcs = (void(**)(int, HTTPreq*))malloc(http->cap * (sizeof(void(*)(int, HTTPreq*))));
    return http;
}

extern void free_http(HTTP *http) {
    free_hashtab(http->tab);
    free(http->host);
    free(http->funcs);
    free(http);
}

extern void handle_http(HTTP *http, char *path, void(*handle)(int, HTTPreq*)) {
    set_hashtab(http->tab, string(path), decimal(http->len));
    http->funcs[http->len] = handle;
    http->len += 1;
    if(http->len == http->cap) {
        http->cap <<= 1;
        http->funcs = (void(**)(int, HTTPreq*))realloc(http->funcs, http->cap * (sizeof(void(*)(int, HTTPreq*))));
    }
}

extern i8 listen_http(HTTP *http) {
    int listener = listen_net(http->host);
    if(listener < 0) {
        return 1;
    } else {
        while(1) {
            int conn = accept_net(listener);
            if(conn < 0) {
                continue;
            }
            HTTPreq req = new_req_();
            while(1) {
                char buffer[BUFSIZ] = {0};
                int n = recv_net(conn, buffer, BUFSIZ);
                if(n < 0) {
                    break;
                }
                parse_req_(&req, buffer, n);
                if(n != BUFSIZ) {
                    break;
                }
            }
            switch_http_(http, conn, &req);
            close_net(conn);
        }
    }
    close_net(listener);
    return 0;
}

extern void parse_html_http(int conn, char* filename) {
    char buffer[BUFSIZ] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
    size_t readsize = strlen(buffer);
    send_net(conn, buffer, readsize);
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "%s\n", "FILE DOESNT EXISTS");
        return;
    }
    while((readsize = fread(buffer, sizeof(char), BUFSIZ, file)) != 0) {
        send_net(conn, buffer, readsize);
    }
    fclose(file);
}