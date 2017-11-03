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

static uv_loop_t *loop;

static void after_read(uv_stream_t *, ssize_t nread, const uv_buf_t *buf);

static void on_close(uv_handle_t *peer);

static void on_connection(uv_stream_t *, int status);

static void parse_package(char* data);

static int libuv_serv();


#endif //SHOWCASE_SOCK_H
