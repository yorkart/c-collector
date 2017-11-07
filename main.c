#include <stdio.h>

#include "utils/bytes.h"
#include "net/buffer.h"
#include "net/frame.h"

static void buffer_test() {
    int i, n;
    struct bytes_buffer *buffer = create_bytes_buffer();

    for (i =1; i<= 100; i++) {
        int len = i+1;
        char* p = (char*)malloc(sizeof(char) * len);
        for(n = 0; n < i; n ++) {
            p[n] = (char)(n + 65);
        }

        p[i] = 0;

//        printf("input string: %s\n", p);
        append_to_bytes_buffer(p, 0, len, buffer);
        printf("length :%d\n" , buffer->length);

        free(p);
    }

    print_buffer(buffer);

    char* bytes = buffer_to_bytes(buffer);
    for(i=0; i< buffer->length; i++) {
        printf("%d,", bytes[i]);
    }
    free(bytes);

    free_buffer(buffer);
}

void test_entry() {
    int n = 0;
    for (n =0; n < 50000; n ++ ) {
        buffer_test();

    }
}

void test_length_parser() {
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

int main() {
//    int n = 10;
//    char * buffer = int_2_bytes(n);
//    printf("buffer: %d,%d,%d,%d\n", (int)buffer[0], (int)buffer[1], (int)buffer[2], (int)buffer[3]);

//    n = bytes_2_int(buffer);

    test_length_parser();

    printf("Hello, World!\n");
    getchar();

    test_entry();

    printf("finish!\n");

    getchar();getchar();
    return 0;
}