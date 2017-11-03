//
// Created by wangyue1 on 2017/11/2.
//

#ifndef SHOWCASE_UTILS_H
#define SHOWCASE_UTILS_H

#include <stdlib.h>
#include "../utils/list.h"

static int bytes_2_int(const char* buf, int offset);

static int bytes_int_1(const char* buf, int offset);
static int bytes_int_2(const char* buf, int offset);
static int bytes_int_3(const char* buf, int offset);
static int bytes_int_4(const char* buf, int offset);

typedef struct {
    int length;
    int (*bytes_int_fn)(const char* , int );
} int_parser;

static int_parser* build_int_parser(int length) {
    int_parser* parser = (int_parser*)malloc(sizeof(int_parser));
    parser->length = length;
//    if (length == 1) {
//        parser->bytes_int_fn = &bytes_int_1;
//    } else if (length == 2) {
//        parser->bytes_int_fn = &bytes_int_2;
//    } else if (length == 3) {
//        parser->bytes_int_fn = &bytes_int_3;
//    } else if (length == 4) {
//        parser->bytes_int_fn = &bytes_int_4;
//    }

    return parser;
}

typedef struct {
    int max_frame_length;
    int length_field_offset;
    int_parser* length_field_length;
    int length_adjustment; /* relative to length offset, body offset position*/
    int initial_bytes_to_strip;
} length_field_based_frame_desc;

typedef struct {
    int message_id;
    signed char type;
    int length;
    char *data;
} net_package;

#endif //SHOWCASE_UTILS_H
