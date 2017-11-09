//
// Created by wangyue1 on 2017/11/7.
//

#ifndef C_COLLECTOR_BYTES_H
#define C_COLLECTOR_BYTES_H

#include <stdlib.h>
#include <stdint.h>

static inline uint64_t bytes_ntohll(uint64_t num) {
    int i = 0x1;
    uint64_t retval;
    if (*(char *) &i == 0x1) {
#if defined (WIN32) || defined (_WIN32)
        retval = _byteswap_uint64(num);
#elif defined(__linux__ )
        retval = __builtin_bswap64(num);
#else
        retval = ((uint64_t)ntohl(num & 0x00000000ffffffffULL)) << 32;
        retval |= ntohl((num & 0xffffffff00000000ULL) >> 32);
#endif
    } else {
        return num;
    }
    return retval;
}

static int bytes_2_int(const unsigned char *bytes) {

    return (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
//    return (int) bytes_ntohll((uint64_t) n);

//    int addr = bytes[0] & 0xFF;
//    addr |= ((bytes[1] << 8) & 0xFF00);
//    addr |= ((bytes[2] << 16) & 0xFF0000);
//    addr |= ((bytes[3] << 24) & 0xFF000000);
//    return addr;
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
