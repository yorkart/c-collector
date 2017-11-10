//
// Created by wangyue1 on 2017/11/10.
//

#ifndef C_COLLECTOR_QUEUE_TEST_H
#define C_COLLECTOR_QUEUE_TEST_H

#include <stdio.h>
#include <stdlib.h>

#include "../../src/utils/lfqueue.h"

void queue_test() {
    long ret;
    struct lfq_ctx ctx;
    lfq_init(&ctx, 1);
    lfq_enqueue(&ctx,(void *)1);
    lfq_enqueue(&ctx,(void *)3);
    lfq_enqueue(&ctx,(void *)5);
    lfq_enqueue(&ctx,(void *)8);
    lfq_enqueue(&ctx,(void *)4);
    lfq_enqueue(&ctx,(void *)6);

    while ( (ret = (long)lfq_dequeue(&ctx)) != 0 ) {
        printf("lfq_dequeue %ld\n", ret);
    }

    lfq_clean(&ctx);
}

#endif //C_COLLECTOR_QUEUE_TEST_H
