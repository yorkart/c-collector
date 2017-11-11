//
// Created by wangyue1 on 2017/11/2.
//

#include <stdio.h>
#include "frame.h"

length_field_based_frame_desc *create_length_field_based_frame_desc(
        int max_frame_length,
        int length_field_offset,
        int length_field_length,
        int length_adjustment,
        int initial_bytes_to_strip) {
    length_field_based_frame_desc *frame_desc = malloc(sizeof(length_field_based_frame_desc));
    frame_desc->max_frame_length = max_frame_length;
    frame_desc->length_field_offset = length_field_offset;
    frame_desc->length_field_length = length_field_length;
    frame_desc->length_adjustment = length_adjustment;
    frame_desc->initial_bytes_to_strip = initial_bytes_to_strip;

    frame_desc->header_length = frame_desc->length_field_offset +
                                frame_desc->length_field_length +
                                frame_desc->length_adjustment;
    return frame_desc;
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

    append_total_size = buffer->length_body + frame_desc->header_length;
    if (buffer->length >= append_total_size) {
        return buffer->length - append_total_size;
    }

    return -1;
}
