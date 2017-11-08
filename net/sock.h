//
// Created by wangyue1 on 2017/11/3.
//

#ifndef SHOWCASE_SOCK_H
#define SHOWCASE_SOCK_H

//typedef struct SRWLOCK {
//    void *ptr;
//} SRWLOCK, *PSRWLOCK;

#include <uv.h>

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

int libuv_serve();

void on_connection(uv_stream_t *server, int status);

void after_read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);

void parse_package(char* data);



#endif //SHOWCASE_SOCK_H
