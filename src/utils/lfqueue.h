//
// Created by wangyue1 on 2017/11/10.
// lock free queue: https://github.com/darkautism/lfqueue
//

#ifndef C_COLLECTOR_QUEUE_H
#define C_COLLECTOR_QUEUE_H

struct lfq_node{
    void * data;
    struct lfq_node * volatile next;
    struct lfq_node * volatile free_next;
    volatile int can_free;
};

struct lfq_ctx{
    volatile struct lfq_node  * volatile head;
    volatile struct lfq_node  * volatile tail;
    int volatile count;
    volatile struct lfq_node * * HP;
    volatile int * tid_map;
    int volatile is_freeing;
    volatile struct lfq_node * volatile fph; // free pool head
    volatile struct lfq_node * volatile fpt; // free pool tail
    int MAXHPSIZE;
};

int lfq_init(struct lfq_ctx *ctx, int max_consume_thread);
int lfq_clean(struct lfq_ctx *ctx);
int lfq_enqueue(struct lfq_ctx *ctx, void * data);
void * lfq_dequeue_tid(struct lfq_ctx *ctx, int tid );
void * lfq_dequeue(struct lfq_ctx *ctx );

#endif //C_COLLECTOR_QUEUE_H
