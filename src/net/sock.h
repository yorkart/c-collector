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

typedef struct {
    void* create_parameter;
    void (*free_buffer)(void* buffer);
    void* (*create_buffer)(void* create_parameter);
    void (*append_data)(void* data, int size, void* buffer);
} net_server_context;

typedef struct {
    uv_stream_t *server;
    void* buffer;
} net_handler_context;

int libuv_serve(net_server_context* server_context);

void on_connection(uv_stream_t *server, int status);

void after_read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);

void parse_package(char* data);



#endif //SHOWCASE_SOCK_H
