//
// Created by wangyue1 on 2017/11/9.
//

#ifndef C_COLLECTOR_TOOLS_H
#define C_COLLECTOR_TOOLS_H

#include <stdlib.h>
#include <stdio.h>

static void print_bytes(char *bytes, int length) {
    char *c = (char *) malloc(sizeof(char) * (length + 1));
    memcpy(c, bytes, (size_t) length);
    c[length] = 0;
    printf("console: %s\n", c);

    free(c);
}

/* Have our own assert, so we are sure it does not get optimized away in
 * a release build.
 */
#define ASSERT(expr)                                      \
 do {                                                     \
  if (!(expr)) {                                          \
    fprintf(stderr,                                       \
            "Assertion failed in %s on line %d: %s\n",    \
            __FILE__,                                     \
            __LINE__,                                     \
            #expr);                                       \
    abort();                                              \
  }                                                       \
 } while (0)

#endif //C_COLLECTOR_TOOLS_H
