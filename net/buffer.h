//
// Created by wangyue1 on 2017/11/3.
//

#ifndef SHOWCASE_BUFFER_H
#define SHOWCASE_BUFFER_H

#include <stdlib.h>
#include "../utils/list.h"
#include "../utils/counter.h"
#include "../utils/bytes.h"

struct bytes_chunk {
    char *data;
    int length;
    struct list_head list;
};

struct bytes_buffer { /* length_total == length mean read finish */
    int length_body;
    int length;
    struct bytes_chunk chunk_list;
};

struct bytes_buffer *create_bytes_buffer();

void append_to_bytes_buffer(char *data, int offset, int length, struct bytes_buffer *buffer);

static void set_buffer_position(int position, struct bytes_buffer *buffer);

char *buffer_to_bytes(struct bytes_buffer *buffer);

void print_buffer(struct bytes_buffer *buffer);

void free_buffer(struct bytes_buffer *buffer);

int read_buffer_int(int offset, int length, int *out_value, struct bytes_buffer *buffer);

#endif //SHOWCASE_BUFFER_H
