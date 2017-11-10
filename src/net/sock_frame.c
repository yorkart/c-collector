//
// Created by wangyue1 on 2017/11/9.
//

#include "sock_frame.h"

net_server_context *create_frame_server_context(sock_frame_arg* frame_arg){
    net_server_context *server_context = malloc(sizeof(net_server_context));
    server_context->create_parameter = (void*)frame_arg;
    server_context->create_buffer = frame_create_buffer;
    server_context->append_data = frame_append_data;
    server_context->free_buffer = frame_free_buffer;

    return server_context;
}

void *frame_create_buffer(void* create_parameter) {
    sock_frame_arg* frame_arg = (sock_frame_arg* )create_parameter;
    sock_frame_buffer* frame_buffer = (sock_frame_buffer*)malloc(sizeof(sock_frame_buffer));

    frame_buffer->buffer = create_bytes_buffer();
    frame_buffer->frame_arg = frame_arg;

    return (void*)frame_buffer;
}

void frame_free_buffer(void *arg) {
    sock_frame_buffer* frame_buffer = (sock_frame_buffer*)arg;

    free_buffer(frame_buffer->buffer);
    free(frame_buffer);
}

void frame_append_data(void *data, int size, void *arg) {
    sock_frame_buffer* frame_buffer = (sock_frame_buffer*)arg;
    struct bytes_buffer *buffer = frame_buffer->buffer;
    length_field_based_frame_desc *frame_desc = frame_buffer->frame_arg->frame_desc;
    struct lfq_ctx *queue = frame_buffer->frame_arg->queue;

    char *unread_buffer;

    int rt = parse_frame(frame_desc, buffer, (char*)data, size);
    printf("parse frame return value: %d, buffer:\n", rt);
    if (rt < 0) {
        return;
    }

    frame_buffer->frame_arg->call_back(queue, frame_desc, buffer);

    free_buffer(buffer);
    frame_buffer->buffer = create_bytes_buffer();

    printf("clear buffer\n");

    if (rt > 0) {
        unread_buffer = (char *) malloc(sizeof(char) * rt);
        memcpy(unread_buffer, data + (size - rt), (size_t)rt);

        frame_append_data(unread_buffer, rt, arg);

        free(unread_buffer);
    }
}