//
// Created by wangyue1 on 2017/11/9.
//

#ifndef C_COLLECTOR_SOCKET_FRAME_H
#define C_COLLECTOR_SOCKET_FRAME_H

#include "buffer.h"
#include "frame.h"
#include "sock.h"
#include "../utils/lfqueue.h"

struct sock_frame_arg {
    length_field_based_frame_desc *frame_desc;
    struct lfq_ctx *queue;
    void (*call_back)(struct sock_frame_arg *frame_arg, struct bytes_buffer* buffer);
};

typedef struct {
    struct bytes_buffer *buffer;
//    length_field_based_frame_desc *frame_desc;
    struct sock_frame_arg *frame_arg;
} sock_frame_buffer;

net_server_context *create_frame_server_context(struct sock_frame_arg* frame_arg);

void *frame_create_buffer(void *create_parameter);

void frame_free_buffer(void *frame_buffer);

void frame_append_data(void *data, int size, void *frame_buffer);

#endif //C_COLLECTOR_SOCKET_FRAME_H
