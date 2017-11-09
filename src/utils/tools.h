//
// Created by wangyue1 on 2017/11/9.
//

#ifndef C_COLLECTOR_TOOLS_H
#define C_COLLECTOR_TOOLS_H

#include <stdlib.h>
#include <stdio.h>

static void print_bytes(char* bytes, int length) {
    char* c = (char*)malloc(sizeof(char) * (length+1));
    memcpy(c, bytes, length);
    c[length] = 0;
    printf("console: %s\n", c);

    free(c);
}

#endif //C_COLLECTOR_TOOLS_H
