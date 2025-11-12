//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (src == NULL || len == 0) {
        return 0;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            return 0;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_ptr = src;
    for (size_t i = 0; i < len; i++) {
        *dest++ = *src_ptr++;
    }
    buf->size = new_size;
    return 1;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    while (in_ptr < in_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        const unsigned char *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        if (out_ptr + 2 > out_end) {
            return 0;
        }
        *out_ptr++ = current;
        *out_ptr++ = (unsigned char)count;
        in_ptr += count;
    }
    return out_ptr - output;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        input[input_len++] = (unsigned char)c;
    }
    if (input_len == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, input_len * 2)) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
    size_t compressed_size = compress_rle(input, input_len, compressed.data, compressed.capacity);
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    compressed.size = compressed_size;
    const unsigned char *out_ptr = compressed.data;
    const unsigned char *out_end = compressed.data + compressed.size;
    while (out_ptr < out_end) {
        if (putchar(*out_ptr) == EOF) {
            fprintf(stderr, "Error: Output write failed\n");
            buffer_free(&compressed);
            return 1;
        }
        out_ptr++;
    }
    buffer_free(&compressed);
    return 0;
}