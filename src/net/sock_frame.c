//
// Created by wangyue1 on 2017/11/9.
//

#include "sock_frame.h"
#include "frame.h"

void *frame_create_buffer(void* create_parameter) {
    length_field_based_frame_desc* frame_desc = (length_field_based_frame_desc*)create_parameter;
    sock_frame_buffer* frame_buffer = (sock_frame_buffer*)malloc(sizeof(sock_frame_buffer));

    frame_buffer->buffer = create_bytes_buffer();
    frame_buffer->frame_desc = (length_field_based_frame_desc*)malloc(sizeof(length_field_based_frame_desc));

//    memcpy(frame_buffer, frame_desc, sizeof(length_field_based_frame_desc));
    frame_buffer->frame_desc->length_field_offset = frame_desc->length_field_offset;
    frame_buffer->frame_desc->length_field_length = frame_desc->length_field_length;
    frame_buffer->frame_desc->length_adjustment = frame_desc->length_adjustment;
    frame_buffer->frame_desc->initial_bytes_to_strip = frame_desc->initial_bytes_to_strip;
    frame_buffer->frame_desc->max_frame_length = frame_desc->max_frame_length;

    return (void*)frame_buffer;
}

void frame_free_buffer(void *arg) {
    sock_frame_buffer* frame_buffer = (sock_frame_buffer*)arg;

    free_buffer(frame_buffer->buffer);
    free(frame_buffer->frame_desc);
    free(frame_buffer);
}

void frame_append_data(void *data, int size, void *arg) {
    sock_frame_buffer* frame_buffer = (sock_frame_buffer*)arg;
    struct bytes_buffer *buffer = frame_buffer->buffer;
    length_field_based_frame_desc *frame_desc = frame_buffer->frame_desc;

    char *unread_buffer;

    int offset = frame_desc->length_field_offset + frame_desc->length_field_length + frame_desc->length_adjustment;
    char *body;

    int rt = parse_frame(frame_desc, buffer, (char*)data, size);
    printf("parse frame return value: %d, buffer:\n", rt);
    if (rt < 0) {
        return;
    }

    body = read_buffer_bytes(offset, buffer->length - offset, buffer);
    printf("body length: %d\n", buffer->length_body);
//    printf("length: %s\n", body);
    print_bytes(body, buffer->length - offset);

    free_buffer(buffer);
    frame_buffer->buffer = create_bytes_buffer();

    free(body);

    printf("clear buffer\n");

    if (rt > 0) {
        unread_buffer = (char *) malloc(sizeof(char) * rt);
        memcpy(unread_buffer, data + (size - rt), (size_t)rt);

        frame_append_data(unread_buffer, rt, arg);

        free(unread_buffer);
    }
}