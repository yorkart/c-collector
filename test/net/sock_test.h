//
// Created by wangyue1 on 2017/11/8.
//

#ifndef C_COLLECTOR_SOCKET_TEST_H
#define C_COLLECTOR_SOCKET_TEST_H

#include <pthread.h>
#include <unistd.h>

#include "../../src/net/sock.h"
#include "../../src/net/frame.h"
#include "../../src/net/sock_frame.h"
#include "../../src/utils/tools.h"

//length_field_based_frame_desc *frame_desc;
//
//void frame_free_buffer(void *buffer);
//
//void *frame_create_buffer();
//
//void frame_append_data(void *data, int size, void *buffer);
//
//void frame_free_buffer(void *buffer) {
//    free_buffer((struct bytes_buffer *) buffer);
//}
//
//void *frame_create_buffer() {
//    return (void *) create_bytes_buffer();
//}
//
//void frame_append_data(void *data, int size, void *buffer) {
//    struct bytes_buffer *buf = (struct bytes_buffer *) buffer;
//    char *unread_buffer;
//
//    int offset = frame_desc->length_field_offset + frame_desc->length_field_length + frame_desc->length_adjustment;
//    char *body;
//
//    int rt = parse_frame(frame_desc, buffer, data, size);
//    printf("parse frame return value: %d, buffer:\n", rt);
//    if (rt < 0) {
//        return;
//    }
//
//    body = read_buffer_bytes(offset, buf->length - offset, buf);
//    printf("length: %d\n", buf->length_body);
////    printf("length: %s\n", body);
//    print_bytes(body, buf->length - offset);
//
//    clear_buffer(buffer);
//
//    if (rt > 0) {
//        unread_buffer = (char *) malloc(sizeof(char) * rt);
//        memcpy(unread_buffer, data + (size - rt), (size_t)rt);
//
//        frame_append_data(unread_buffer, rt, buffer);
//    }
//}

void call_back(struct lfq_ctx* queue, length_field_based_frame_desc *frame_desc, struct bytes_buffer* buffer) {
    int offset = frame_desc->length_field_offset + frame_desc->length_field_length + frame_desc->length_adjustment;
    char *body = read_buffer_bytes(offset, buffer->length - offset, buffer);
    printf("body length: %d\n", buffer->length_body);
//    printf("length: %s\n", body);
    print_bytes(body, buffer->length - offset);

    free(body);
}

void *serve(void *arg) {
    struct lfq_ctx *queue = (struct lfq_ctx *) arg;
    length_field_based_frame_desc *frame_desc = malloc(sizeof(length_field_based_frame_desc));
    sock_frame_arg *frame_arg = (sock_frame_arg *) malloc(sizeof(sock_frame_arg));
    net_server_context *server_context;

    frame_desc->length_field_offset = 5;
    frame_desc->length_field_length = 4;
    frame_desc->length_adjustment = 0;
    frame_desc->max_frame_length = 1024 * 1024;

    frame_arg->frame_desc = frame_desc;
    frame_arg->queue = queue;
    frame_arg->call_back = call_back;

    server_context = create_frame_server_context(frame_arg);

    printf("server start......");
    libuv_serve(server_context);

    return 0;
}

void* consume(void* arg) {
    struct lfq_ctx *queue = (struct lfq_ctx *) arg;
    void* ret ;
    for(;;) {
        ret = lfq_dequeue(queue);
        if(ret == 0) {
            sleep(1000);
        } else {
            sleep(100);
        }
    }

    return 0;
}

int libuv_serve_test() {
    pthread_t _thread1;
    pthread_t _thread2;

    struct lfq_ctx *queue = (struct lfq_ctx *) malloc(sizeof(struct lfq_ctx));
    lfq_init(queue, 1);

    pthread_create(&_thread1, NULL, serve, queue);
    pthread_create(&_thread2, NULL, consume, queue);

    pthread_join(_thread1, NULL);
    pthread_join(_thread2, NULL);
}

#endif //C_COLLECTOR_SOCKET_TEST_H
