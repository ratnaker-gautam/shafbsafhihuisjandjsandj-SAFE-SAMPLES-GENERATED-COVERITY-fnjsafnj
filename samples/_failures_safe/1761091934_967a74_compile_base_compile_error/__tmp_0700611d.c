//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0 || src == NULL) {
        return;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + len) {
            if (new_capacity > MAX_OUTPUT_SIZE / 2) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        if (buf->size + len > new_capacity) {
            return;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    while (in_ptr < in_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        const unsigned char *scan = in_ptr + 1;
        while (scan < in_end && *scan == current && count < 255) {
            count++;
            scan++;
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

size_t decompress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0 || input_size % 2 != 0) {
        return 0;
    }
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    while (in_ptr < in_end) {
        unsigned char value = *in_ptr++;
        unsigned char count = *in_ptr++;
        if (out_ptr + count > out_end) {
            return 0;
        }
        for (unsigned char i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, 64);
    buffer_init(&compressed_buf, 64);
    buffer_init(&decompressed_buf, 64);
    unsigned char chunk[64];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, sizeof(chunk), stdin)) > 0) {
        buffer_append(&input_buf, chunk, bytes_read);
        if (input_buf.size >= MAX_INPUT_SIZE) {
            break;
        }
    }
    if (input_buf.size == 0) {
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    unsigned char *compressed_data = malloc(MAX_OUTPUT_SIZE);
    if (compressed_data == NULL) {
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        free(compressed_data);
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    buffer_append(&compressed_buf, compressed_data, compressed_size);