//
// Created by wangyue1 on 2017/11/8.
//

#ifndef C_COLLECTOR_FRAME_TEST_H
#define C_COLLECTOR_FRAME_TEST_H

#include <stdio.h>
#include "../../src/net/frame.h"

void length_parser_test() {
    int n;
    int rt ;
    char* a;
    struct bytes_buffer *buffer = create_bytes_buffer();
    char* c1 = (char*)malloc(sizeof(char) * 3);
    char* c2 = (char*)malloc(sizeof(char) * 3);
    char* c3 = (char*)malloc(sizeof(char) * 2);
    char* c = (char*)malloc(sizeof(char) * 11);

    length_field_based_frame_desc frame_desc;
    frame_desc.length_field_offset = 4;
    frame_desc.length_field_length = 4;
    frame_desc.length_adjustment = 0;
    frame_desc.max_frame_length = 100;

    c1[0] = 10;
    c1[1] = 0;
    c1[2] = 0;
    c2[0] = 0;
    c2[1] = 10;
    c2[2] = 0;
    c3[0] = 0;
    c3[1] = 0;

    memset(c, 65, 11);
    c[10] = 0;

//    append_to_bytes_buffer(c1, 0, 3, buffer);
//    append_to_bytes_buffer(c2, 0, 3, buffer);
//    append_to_bytes_buffer(c3, 0, 3, buffer);
//
//    read_buffer_int(4, 4, &n ,buffer);


    rt = parse_frame(&frame_desc, buffer, c1, 3);
    rt = parse_frame(&frame_desc, buffer, c2, 3);
    rt = parse_frame(&frame_desc, buffer, c3, 2);
    rt = parse_frame(&frame_desc, buffer, c, 11);

    if (rt > 0) {
        a = (char*)malloc(sizeof(char) * rt);
        memcpy(a, c + (11 - rt), (size_t )rt);
    }
    printf("end %d", rt);
}

#endif //C_COLLECTOR_FRAME_TEST_H
