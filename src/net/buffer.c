//
// Created by wangyue1 on 2017/11/3.
//

#include <mem.h>
#include <stdio.h>
#include "buffer.h"

struct bytes_buffer *create_bytes_buffer() {
    struct bytes_buffer *buffer = (struct bytes_buffer *) malloc(sizeof(struct bytes_buffer));
    buffer->length_body = -1;
    buffer->length = 0;
    buffer->chunks = 0;

    INIT_LIST_HEAD(&buffer->chunk_list.list);

    return buffer;
}

void append_to_bytes_buffer(char *data, int offset, int length, struct bytes_buffer *buffer) {
    char *buf = (char *) malloc(sizeof(char) * length);
    memcpy(buf, data + offset, (size_t) length);

    struct bytes_chunk *chunk = (struct bytes_chunk *) malloc(sizeof(struct bytes_buffer));
    chunk->data = buf;
    chunk->length = length;
    chunk->id = buffer->chunks;

    list_add(&(chunk->list), &(buffer->chunk_list.list));

    buffer->length += length;
    buffer->chunks = buffer->chunks + 1;
}

char *buffer_to_bytes(struct bytes_buffer *buffer) {
    struct list_head *pos;
    struct bytes_chunk *tmp;
    char *bytes = (char *) malloc(sizeof(char) * buffer->length);
    char *current_pos = bytes;

    list_for_each_prev(pos, &(buffer->chunk_list.list)) {
        tmp = list_entry(pos, struct bytes_chunk, list);
        memcpy(current_pos, tmp->data, (size_t) tmp->length);
        current_pos = current_pos + tmp->length;
    }

    return bytes;
}

void print_buffer(struct bytes_buffer *buffer) {
    int i = 0, n = 0;
    struct list_head *pos;
    struct bytes_chunk *tmp;

    list_for_each_prev(pos, &(buffer->chunk_list.list)) {
        printf("%3d ", n++);

        tmp = list_entry(pos, struct bytes_chunk, list);

        printf("length: %d, data: ", tmp->length);
        for (i = 0; i < tmp->length; i++) {
            printf("%d,", (int) (tmp->data[i]));
        }

        printf("\n");
    }
}

void clear_buffer(struct bytes_buffer *buffer) {
    struct list_head *pos, *q;
    struct bytes_chunk *tmp;

    list_for_each_safe(pos, q, &(buffer->chunk_list.list)) {
        tmp = list_entry(pos, struct bytes_chunk, list);
        list_del(pos);
        free(tmp->data);
        free(tmp);
    }

    buffer->length_body = -1;
    buffer->length = 0;

    INIT_LIST_HEAD(&buffer->chunk_list.list);
}

void free_buffer(struct bytes_buffer *buffer) {
    struct list_head *pos, *q;
    struct bytes_chunk *tmp;

    list_for_each_safe(pos, q, &(buffer->chunk_list.list)) {
        tmp = list_entry(pos, struct bytes_chunk, list);
        list_del(pos);
        free(tmp->data);
        free(tmp);
    }

    free(buffer);
}


int read_buffer_int(int offset, int length, int *out_value, struct bytes_buffer *buffer) {
    struct list_head *pos;
    struct bytes_chunk *tmp;

    int finish_chunk_length = 0;
    int current_offset = offset;
    int max_offset = offset + length;

    unsigned char *bytes = (unsigned char *) malloc(sizeof(unsigned char) * 4);
    int bytes_index = 0;
//    if (length <= 4) {
//        (char *) malloc(sizeof(char) * 4);
//    } else {
//        return -1;
//    }

    list_for_each_prev(pos, &(buffer->chunk_list.list)) {
        tmp = list_entry(pos, struct bytes_chunk, list);
        while (current_offset < max_offset) {
            if (tmp->length + finish_chunk_length > current_offset) {
                bytes[bytes_index] = (unsigned char) tmp->data[current_offset - finish_chunk_length];
                bytes_index++;
                current_offset++;
            } else {
                break;
            }
        }

        if (current_offset == max_offset) {
            break;
        }

        finish_chunk_length += tmp->length;
    }

    *out_value = bytes_2_int(bytes);
    free(bytes);

    return 0;
}

//char *read_buffer_bytes(int offset, int length, struct bytes_buffer *buffer) {
//    struct list_head *pos;
//    struct bytes_chunk *tmp;
//
//    int right_boundary = offset + length;
//
//    int finish_chunk_length = 0;
//    int current_offset = offset;
//
//    int read_chunk_length;
//    int max_length;
//
//    char *bytes = (char *) malloc(sizeof(char) * length);
//    int bytes_index = 0;
//
//    list_for_each_prev(pos, &(buffer->chunk_list.list)) {
//        tmp = list_entry(pos, struct bytes_chunk, list);
//
//        printf("chunk index: %d\n", tmp->id);
////        print_bytes(tmp->data + (tmp->id == 0? 9 : 0), tmp->length - (tmp->id == 0? 9 : 0));
//
//        max_length = tmp->length + finish_chunk_length;
//        finish_chunk_length = max_length;
//
//        if (max_length < offset) {
//            continue;
//        }
//
//        read_chunk_length = max_length - current_offset;
//        memcpy(bytes + bytes_index, tmp->data + (tmp->length - read_chunk_length), (size_t) read_chunk_length);
//
//        if (max_length > right_boundary) {
//            break;
//        }
//
//        current_offset += read_chunk_length;
//        bytes_index += read_chunk_length;
//    }
//
//    return bytes;
//}