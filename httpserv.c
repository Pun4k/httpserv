#include "httpserv.h"

int main() {
    printf("Hello World!\n");
    HTTP *server = new_http("127.0.0.1:8080");

    handle_http(server, "/", index_page);
    handle_http(server, "/about/", about_page);

    listen_http(server);
    free_http(server);
    return 0;
}

extern void index_page(int conn, HTTPreq *req) {
    if(strcmp(req->path, "/") != 0) {
        parse_html_http(conn, "../pages/page404.html");
        return;
    }
    parse_html_http(conn, "../pages/index.html");
}

extern void about_page(int conn, HTTPreq *req) {
    if(strcmp(req->path, "/about/") != 0) {
        parse_html_http(conn, "../pages/page404.html");
        return;
    }
    parse_html_http(conn, "../pages/about.html");
}
