//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define MAX_OUTPUT_SIZE 2097152

typedef struct {
    uint8_t *data;
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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (data_size == 0 || data == NULL) {
        return 0;
    }
    if (buf->size > SIZE_MAX - data_size) {
        return 0;
    }
    size_t new_size = buf->size + data_size;
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
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

size_t read_input(uint8_t *buffer, size_t max_size) {
    size_t total_read = 0;
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF) {
            break;
        }
        buffer[total_read] = (uint8_t)c;
        total_read++;
    }
    return total_read;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) {
        return 0;
    }
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        uint8_t run[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run, 2)) {
            return 0;
        }
        i += count;
    }
    return 1;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = read_input(input, MAX_INPUT_SIZE);
    if (input_size == 0) {
        fprintf(stderr, "Error: No input data\n");
        return 1;
    }
    Buffer compressed;
    if (!buffer_init(&compressed, input_size * 2)) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
    if (!compress_rle(input, input_size, &compressed)) {
        fprintf(stderr, "Error: Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    for (size_t i = 0; i < compressed.size; i++) {
        if (putchar(compressed.data[i]) == EOF) {
            fprintf(stderr, "Error: Output write failed\n");
            buffer_free(&compressed);
            return 1;
        }
    }
    buffer_free(&compressed);
    return 0;
}