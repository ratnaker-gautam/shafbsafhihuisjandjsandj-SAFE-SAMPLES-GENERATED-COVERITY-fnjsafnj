//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
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
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) {
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
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    uint8_t *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size = new_size;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    while (in_ptr < in_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        if (out_ptr + 2 > out_end) {
            return 0;
        }
        *out_ptr = current;
        out_ptr++;
        *out_ptr = (uint8_t)count;
        out_ptr++;
        in_ptr += count;
    }
    return out_ptr - output;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && input_len < MAX_INPUT_SIZE) {
        input[input_len] = (uint8_t)c;
        input_len++;
    }
    if (input_len == 0) {
        fputs("No input data\n", stderr);
        return 1;
    }
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fputs("Compression failed\n", stderr);
        return 1;
    }
    Buffer output_buf;
    if (!buffer_init(&output_buf, 1024)) {
        fputs("Buffer initialization failed\n", stderr);
        return 1;
    }
    uint8_t header[4];
    header[0] = 'R';
    header[1] = 'L';
    header[2] = 'E';
    header[3] = 0x01;
    if (!buffer_append(&output_buf, header, 4)) {
        fputs("Header append failed\n", stderr);
        buffer_free(&output_buf);
        return 1;
    }
    uint8_t size_bytes[4];
    size_bytes[0] = (input_len >> 24) & 0xFF;
    size_bytes[1] = (input_len >> 16) & 0xFF;
    size_bytes[2] = (input_len >> 8) & 0xFF;
    size_bytes[3] = input_len & 0xFF;
    if (!buffer_append(&output_buf, size_bytes, 4)) {
        fputs("Size append failed\n", stderr);
        buffer_free(&output_buf);
        return 1;
    }
    if (!buffer_append(&output_buf, compressed, compressed_size)) {
        fputs("Data append failed\n", stderr);
        buffer_free(&output_buf);
        return 1;
    }
    for (size_t i = 0; i < output_buf.size; i++) {
        putchar(*(output