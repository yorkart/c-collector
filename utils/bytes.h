//
// Created by wangyue1 on 2017/11/7.
//

#ifndef C_COLLECTOR_BYTES_H
#define C_COLLECTOR_BYTES_H

#include <stdlib.h>

static int bytes_2_int(const char *bytes) {
    int addr = bytes[0] & 0xFF;
    addr |= ((bytes[1] << 8) & 0xFF00);
    addr |= ((bytes[2] << 16) & 0xFF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

static char *int_2_bytes(const int i) {
    char *bytes = (char *) malloc(sizeof(char) * 4);
    bytes[0] = (char) (0xff & i);
    bytes[1] = (char) ((0xff00 & i) >> 8);
    bytes[2] = (char) ((0xff0000 & i) >> 16);
    bytes[3] = (char) ((0xff000000 & i) >> 24);

    return bytes;
}

#endif //C_COLLECTOR_BYTES_H
