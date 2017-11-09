//
// Created by wangyue1 on 2017/11/2.
//

#include <stdio.h>
#include "frame.h"

static char *parse_frame_length_bytes(length_field_based_frame_desc *frame_desc,
                                      struct bytes_buffer *buffer) {

}

static int parse_frame_length(length_field_based_frame_desc *frame_desc,
                              struct bytes_buffer *buffer) {
    // left boundary
    int length_field_offset = frame_desc->length_field_offset;
    // right boundary
    int length_field_boundary = frame_desc->length_field_offset +
                                frame_desc->length_field_length;
    int length_value;

    // check header of the buffer is already
    if (length_field_boundary > buffer->length) {
        return 1;
    }

    // check body length is beyond the threshold
    read_buffer_int(length_field_offset, frame_desc->length_field_length, &length_value, buffer);

    if (length_value > frame_desc->max_frame_length) {
        return -1;
    }

    buffer->length_body = length_value;
    return 0;
}

/**
 * parse frame from socket stream
 * eg: |header-x-part |length (length_field_length) |header-y-part    |body|
 *     offset         length_field_offset           length_adjustment
 * @param frame_desc
 * @param data
 * @param offset
 * @param buf_size
 * @param package
 * @return
 */
int parse_frame(length_field_based_frame_desc *frame_desc,
                struct bytes_buffer *buffer,
                char *data,
                int buf_size) {
    int append_total_size;

    int i;
    for(i=0; i< 9; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    append_to_bytes_buffer(data, 0, buf_size, buffer);

    if (buffer->length_body < 0) {
        parse_frame_length(frame_desc, buffer);

        if (buffer->length_body < 0) {
            return -1;
        }
    }

    append_total_size = buffer->length_body +
                        frame_desc->length_field_offset +
                        frame_desc->length_field_length +
                        frame_desc->length_adjustment;
    if (buffer->length >= append_total_size) {
        return buffer->length - append_total_size;
    }

    return -1;
}

static int parse_package0(length_field_based_frame_desc *frame_desc,
                          const char *buf,
                          const int offset,
                          const int buf_size,
                          net_package *package) {
    if (offset + 4 >= buf_size) {/* 4 */
        return -1;
    }
    int message_id = bytes_2_int(buf + offset);

    if (offset + 5 >= buf_size) {/* 4+1 */
        return -1;
    }
    char type = buf[offset + 4];

    if (offset + 9 >= buf_size) { /* 4+1+4 */
        return -1;
    }
    int length = bytes_2_int(buf + offset + 5);

    if (offset + 9 + length >= buf_size) { /* 4+1+4 */
        return -1;
    }

    char data[length];
    memcpy(data, buf + offset + 9, (size_t) length);

    package->message_id = message_id;
    package->type = type;
    package->length = length;
    package->data = data;

    return 0;
}