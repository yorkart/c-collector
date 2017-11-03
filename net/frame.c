//
// Created by wangyue1 on 2017/11/2.
//

#include <mem.h>
#include "frame.h"

static int bytes_2_int(const char *buf, int offset) {
    int addr = buf[offset] & 0xFF;
    addr |= ((buf[offset + 1] << 8) & 0xFF00);
    addr |= ((buf[offset + 2] << 16) & 0xFF0000);
    addr |= ((buf[offset + 3] << 24) & 0xFF000000);
    return addr;
}

static int bytes_int_1(const char* buf, int offset) {}
static int bytes_int_2(const char* buf, int offset) {}
static int bytes_int_3(const char* buf, int offset) {}
static int bytes_int_4(const char* buf, int offset) {}
/**
 * parse frame from socket stream
 * eg: |header-x-part |length (length_field_length) |header-y-part    |body|
 *     offset         length_field_offset           length_adjustment
 * @param frame_desc
 * @param buf
 * @param offset
 * @param buf_size
 * @param package
 * @return
 */
static int parse_frame(length_field_based_frame_desc *frame_desc,
                       const char *buf,
                       const int offset,
                       const int buf_size,
                       net_package *package) {
    // left boundary
    int length_field_offset = offset + frame_desc->length_field_offset;
    // right boundary
    int length_field_boundary = frame_desc->length_field_offset +
                                frame_desc->length_field_length->length +
                                frame_desc->length_adjustment;

    // check header of the buffer is already
    if (offset + length_field_boundary > buf_size) {
        return -1;
    }

    // check body length is beyond the threshold
    int length = frame_desc->length_field_length->bytes_int_fn(buf, length_field_offset);
    if (length > frame_desc->max_frame_length) {
        return -2;
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
    int message_id = bytes_2_int(buf, offset);

    if (offset + 5 >= buf_size) {/* 4+1 */
        return -1;
    }
    char type = buf[offset + 4];

    if (offset + 9 >= buf_size) { /* 4+1+4 */
        return -1;
    }
    int length = bytes_2_int(buf, offset + 5);

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