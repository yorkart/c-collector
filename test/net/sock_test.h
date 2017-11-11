//
// Created by wangyue1 on 2017/11/8.
//

#ifndef C_COLLECTOR_SOCKET_TEST_H
#define C_COLLECTOR_SOCKET_TEST_H

#include <pthread.h>
#include <unistd.h>


#include "../../src/zlog/zlog.h"
#include "../../src/net/sock.h"
#include "../../src/net/frame.h"
#include "../../src/net/sock_frame.h"
#include "../../src/utils/tools.h"

void call_back(struct sock_frame_arg *frame_arg, struct bytes_buffer *buffer) {
    struct lfq_ctx *queue = frame_arg->queue;

    lfq_enqueue(queue, buffer);

//    int offset = frame_desc->length_field_offset + frame_desc->length_field_length + frame_desc->length_adjustment;
//    char *body = read_buffer_bytes(offset, buffer->length - offset, buffer);
//    printf("body length: %d\n", buffer->length_body);
//    print_bytes(body, buffer->length - offset);
//
//    free(body);
}

void *serve(void *arg) {
    struct lfq_ctx *queue = (struct lfq_ctx *) arg;
    struct sock_frame_arg *frame_arg = (struct sock_frame_arg *) malloc(sizeof(struct sock_frame_arg));
    net_server_context *server_context;
    length_field_based_frame_desc *frame_desc = create_length_field_based_frame_desc(1024 * 1024, 5, 4, 0, 0);

    frame_arg->frame_desc = frame_desc;
    frame_arg->queue = queue;
    frame_arg->call_back = call_back;

    server_context = create_frame_server_context(frame_arg);

    printf("server start......\n");
    libuv_serve(server_context);

    return 0;
}

void *consume(void *arg) {
    struct lfq_ctx *queue = (struct lfq_ctx *) arg;
    void *ret;
    int offset = 9;
    int running = 1;
    sleep(10);
    while (running) {
        ret = lfq_dequeue(queue);
        if (ret == 0) {
            printf("queue empty\n");
            sleep(3);
            continue;
        }

        struct bytes_buffer *buffer = (struct bytes_buffer *) ret;

        sleep(1);

        if (buffer->length_body > buffer->length - offset) {
            printf("error");
        }

//        char *body = read_buffer_bytes(offset, buffer->length_body, buffer);
        char *data = buffer_to_bytes(buffer);

//        printf("body length: %d\n", buffer->length_body);
        zlogf_time(ZLOG_LOC, "body length: %d\n", buffer->length_body);

        print_bytes(data + 9 + (buffer->length_body - 20), 20);

        free(data);
        free_buffer(buffer);
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
