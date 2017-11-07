//
// Created by wangyue1 on 2017/11/2.
//

#ifndef SHOWCASE_UTILS_H
#define SHOWCASE_UTILS_H

#include <stdlib.h>
#include <mem.h>
#include "buffer.h"
#include "../utils/list.h"

typedef struct {
    int max_frame_length;
    int length_field_offset;
    int length_field_length;
    int length_adjustment; /* relative to length offset, body offset position*/
    int initial_bytes_to_strip;
} length_field_based_frame_desc;

typedef struct {
    int message_id;
    signed char type;
    int length;
    char *data;
} net_package;

int parse_frame(length_field_based_frame_desc *frame_desc,
                struct bytes_buffer *buffer,
                char *data,
                int buf_size);

#endif //SHOWCASE_UTILS_H
