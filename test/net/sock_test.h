//
// Created by wangyue1 on 2017/11/8.
//

#ifndef C_COLLECTOR_SOCKET_TEST_H
#define C_COLLECTOR_SOCKET_TEST_H

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

int libuv_serve_test() {
    length_field_based_frame_desc *frame_desc = malloc(sizeof(length_field_based_frame_desc));
    net_server_context *server_context = malloc(sizeof(net_server_context));

    frame_desc->length_field_offset = 5;
    frame_desc->length_field_length = 4;
    frame_desc->length_adjustment = 0;
    frame_desc->max_frame_length = 1024 * 1024;

    server_context->create_parameter = (void*)frame_desc;
    server_context->create_buffer = frame_create_buffer;
    server_context->append_data = frame_append_data;
    server_context->free_buffer = frame_free_buffer;


    return libuv_serve(server_context);
}

#endif //C_COLLECTOR_SOCKET_TEST_H
