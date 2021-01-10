#ifndef HTTPSERV
#define HTTPSERV

#define VERSION 0.0.1

#include <stdio.h>

#include "src/http/http.h"
#include "src/test/test.h"

extern void index_page(int conn, HTTPreq *req);
extern void about_page(int conn, HTTPreq *req);

#endif //!HTTPSERV