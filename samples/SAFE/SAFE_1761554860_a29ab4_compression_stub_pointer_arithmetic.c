//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

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
            if (new_capacity > MAX_OUTPUT_SIZE) {
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
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    unsigned char *output_start = output;
    unsigned char *output_end = output + output_capacity;
    const unsigned char *input_end = input + input_size;
    const unsigned char *current = input;
    while (current < input_end && output < output_end - 2) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;
        while (next < input_end && *next == value && count < 255) {
            count++;
            next++;
        }
        *output++ = (unsigned char)count;
        *output++ = value;
        current = next;
    }
    return output - output_start;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && input_size < MAX_INPUT_SIZE) {
        input[input_size++] = (unsigned char)c;
    }
    if (input_size == 0) {
        return EXIT_SUCCESS;
    }
    Buffer compressed;
    buffer_init(&compressed, input_size * 2);
    if (compressed.data == NULL) {
        return EXIT_FAILURE;
    }
    size_t compressed_size = compress_rle(input, input_size, compressed.data, compressed.capacity);
    if (compressed_size > 0) {
        compressed.size = compressed_size;
        unsigned char *ptr = compressed.data;
        unsigned char *end = ptr + compressed.size;
        while (ptr < end) {
            putchar(*ptr++);
        }
    }
    buffer_free(&compressed);
    return EXIT_SUCCESS;
}