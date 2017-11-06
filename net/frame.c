//
// Created by wangyue1 on 2017/11/2.
//

#include <mem.h>
#include "frame.h"

static int bytes_2_int(const char *buf) {
    int addr = buf[0] & 0xFF;
    addr |= ((buf[1] << 8) & 0xFF00);
    addr |= ((buf[2] << 16) & 0xFF0000);
    addr |= ((buf[3] << 24) & 0xFF000000);
    return addr;
}

static int bytes_int_1(const char* buf) {
    return buf[0];
}
static int bytes_int_2(const char* buf) {
    int addr = buf[0] & 0xFF;
    addr |= ((buf[1] << 8) & 0xFF00);
    return addr;
}
static int bytes_int_3(const char* buf) {
    int addr = buf[0] & 0xFF;
    addr |= ((buf[1] << 8) & 0xFF00);
    addr |= ((buf[2] << 16) & 0xFF0000);
    return addr;
}
static int bytes_int_4(const char* buf) {
    int addr = buf[0] & 0xFF;
    addr |= ((buf[1] << 8) & 0xFF00);
    addr |= ((buf[2] << 16) & 0xFF0000);
    addr |= ((buf[3] << 24) & 0xFF000000);
    return addr;
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
static int parse_frame(length_field_based_frame_desc *frame_desc,
                       struct bytes_buffer* buffer,
                       char *data,
                       int buf_size) {
    // left boundary
    int length_field_offset = frame_desc->length_field_offset;
    // right boundary
    int length_field_boundary = frame_desc->length_field_offset +
                                frame_desc->length_field_length->length;
    int data_field_offset;

    // check header of the buffer is already
    if (length_field_boundary > buf_size) {
        append_to_bytes_buffer(data, 0, buf_size, buffer);
        return -1;
    }

    // check body length is beyond the threshold
    int length = frame_desc->length_field_length->bytes_int_fn(data + length_field_offset);
    if (length > frame_desc->max_frame_length) {
        return -2;
    }

    buffer->length_total = length;

    data_field_offset = length_field_boundary + frame_desc->length_adjustment;
    if (buf_size >= data_field_offset) {
        append_to_bytes_buffer(data, data_field_offset, length, buffer);
    } else { // all remaining
        append_to_bytes_buffer(data, data_field_offset, buf_size - data_field_offset, buffer);
    }

    return 0;
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