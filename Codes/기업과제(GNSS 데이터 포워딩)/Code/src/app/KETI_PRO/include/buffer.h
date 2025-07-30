#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#ifndef _D_HEADER_BUFFER
#define _D_HEADER_BUFFER

#define _GNU_SOURCE
#define PRO_DEFAULT_BUFFER_ARRAY_SIZE 128

struct pro_buffer_status_t {
    int buffer_size_max;
    int buffer_size_now;
    bool buffer_pop_flag;
    bool buffer_push_flag;
};

struct pro_default_buffer_array_t {
    void *data;
    size_t size;
    int id;

    struct pro_default_buffer_array_t *next;
    struct pro_default_buffer_array_t *prev;
};

struct pro_buffer_t {
    struct pro_buffer_status_t status;
    struct pro_default_buffer_array_t *front;
    struct pro_default_buffer_array_t *array;
};

struct buffer_array_setup_t {
    int buffer_size;

    struct pro_buffer_t *buffer_ptr;
};

#endif //?_D_HEADER_BUFFER

extern void *Pro_Buffer_Array_Init(struct buffer_array_setup_t *setup);
extern int Pro_Buffer_Array_Push(void *data, size_t size, int *id, struct pro_buffer_t *user_buffer);
extern int Pro_Buffer_Array_Pop(struct pro_default_buffer_array_t *out_buffer, struct pro_buffer_t *user_buffer);
