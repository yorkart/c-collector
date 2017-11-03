#include <stdio.h>

#include "net/buffer.c"

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

int main() {
    printf("Hello, World!\n");
    getchar();

    test_entry();

    printf("finish!\n");

    getchar();getchar();
    return 0;
}