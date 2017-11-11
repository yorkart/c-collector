#include <stdio.h>

#include "src/zlog/zlog.h"
#include "test/net/buffer_test.h"
#include "test/net/frame_test.h"
#include "test/net/sock_test.h"
#include "test/utils/lfqueue_test.h"


int main() {
    zlog_init("/data/logs/zlog.log");
    zlog_init_flush_thread();
//    int n = 10;
//    char * buffer = int_2_bytes(n);
//    printf("buffer: %d,%d,%d,%d\n", (int)buffer[0], (int)buffer[1], (int)buffer[2], (int)buffer[3]);

//    n = bytes_2_int(buffer);

    queue_test();

    libuv_serve_test();

    length_parser_test();

    printf("Hello, World!\n");
    getchar();

    test_entry();

    printf("finish!\n");

    getchar();getchar();

    zlog_finish();
    return 0;
}