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
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    while (input < end) {
        unsigned char current = *input;
        size_t count = 1;
        const unsigned char *next = input + 1;
        while (next < end && *next == current && count < 255) {
            count++;
            next++;
        }
        if (out_ptr + 2 > out_end) {
            break;
        }
        *out_ptr++ = (unsigned char)count;
        *out_ptr++ = current;
        input += count;
    }
    return out_ptr - output;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t bytes_read = 0;
    int c;
    while (bytes_read < MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        input[bytes_read++] = (unsigned char)c;
    }
    if (bytes_read == 0) {
        return EXIT_FAILURE;
    }
    Buffer compressed;
    buffer_init(&compressed, bytes_read * 2);
    if (compressed.data == NULL) {
        return EXIT_FAILURE;
    }
    size_t compressed_size = compress_rle(input, bytes_read, compressed.data, compressed.capacity);
    if (compressed_size == 0) {
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    compressed.size = compressed_size;
    for (size_t i = 0; i < compressed.size; i++) {
        if (putchar(compressed.data[i]) == EOF) {
            buffer_free(&compressed);
            return EXIT_FAILURE;
        }
    }
    buffer_free(&compressed);
    return EXIT_SUCCESS;
}