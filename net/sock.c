//
// Created by wangyue1 on 2017/11/3.
//

#include "sock.h"

static int libuv_serv() {
    loop = uv_default_loop();

    struct sockaddr_in bind_addr;
    int rt = uv_ip4_addr("10.101.22.31", 8998, &bind_addr);
    if (rt) {
        printf("ip address error\n");
        return 0;
    }

    uv_tcp_t server;
    rt = uv_tcp_init(loop, &server);
    if (rt) {
        printf("tcp init error\n");
        return 0;
    }

    rt = uv_tcp_bind(&server, (const struct sockaddr *) &bind_addr, 0);
    if (rt) {
        printf("tcp bind error\n");
        return 0;
    }

    rt = uv_listen((uv_stream_t *) &server, 128, on_connection);
    if (rt) {
//        fprintf(stderr, "Listen error %s\n", uv_err_name(uv_last_error(loop)));
        printf("tcp listen error\n");
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}

static void echo_alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char *) malloc(suggested_size);
    buf->len = suggested_size;
}

static void on_close(uv_handle_t *peer) {
    free(peer);
}

static void on_connection(uv_stream_t *server, int status) {
    uv_stream_t *stream;
    int r;

    if (status) {
        printf("connection status error\n");
        return;
    }

    stream = (uv_stream_t *) malloc(sizeof(uv_tcp_t));
    r = uv_tcp_init(loop, (uv_tcp_t *) stream);
    if (r) {
        printf("connection tcp init error\n");
        return;
    }

    stream->data = server;

    r = uv_accept(server, stream);
    if (r) {
        printf("connection tcp accept error\n");
        return;
    }

    r = uv_read_start(stream, echo_alloc, after_read);
    if (r) {
        printf("connection tcp read error\n");
        return;
    }

    printf("connection success\n");
}

static void after_shutdown(uv_shutdown_t *req, int status) {
    uv_close((uv_handle_t *) req->handle, on_close);
    free(req);
}

static void after_read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf) {
    int i;
//    write_req_t *wr;
    uv_shutdown_t *sreq;

    if (nread < 0) {
        if (nread != UV_EOF) {
            printf("reading...\n");
            return;
        }

        free(buf->base);
        sreq = (uv_shutdown_t *) malloc(sizeof *sreq);
        if (uv_shutdown(sreq, handle, after_shutdown)) {
            printf("shutdown error\n");
        }
        return;
    }

    if (nread == 0) {
        /* Everything OK, but nothing read. */
        free(buf->base);
        return;
    }

    printf("read data:%d->", (int)nread);
    for (i =0; i< nread; i++) {
        printf(", %c\n", buf->base[i]);
    }
    printf("\n");
}
