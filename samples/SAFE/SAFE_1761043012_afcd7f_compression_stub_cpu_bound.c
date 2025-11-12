//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer* buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_INPUT_SIZE) {
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

void buffer_free(Buffer* buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (data_size > SIZE_MAX - buf->size) {
        return 0;
    }
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return 0;
        }
        uint8_t* new_data = realloc(buf->data, new_capacity);
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

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input_size == 0) {
        return;
    }
    uint8_t current = input[0];
    size_t count = 1;
    for (size_t i = 1; i < input_size; i++) {
        if (input[i] == current && count < 255) {
            count++;
        } else {
            uint8_t run[2] = {current, (uint8_t)count};
            if (!buffer_append(output, run, 2)) {
                return;
            }
            current = input[i];
            count = 1;
        }
    }
    uint8_t run[2] = {current, (uint8_t)count};
    buffer_append(output, run, 2);
}

int read_input(Buffer* input) {
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, chunk, bytes_read)) {
            return 0;
        }
    }
    if (ferror(stdin)) {
        return 0;
    }
    return 1;
}

int main(void) {
    Buffer input, output;
    if (!buffer_init(&input, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    if (!buffer_init(&output, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    if (!read_input(&input)) {
        fprintf(stderr, "Input reading failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    simple_compress(input.data, input.size, &output);
    if (fwrite(output.data, 1, output.size, stdout) != output.size) {
        fprintf(stderr, "Output writing failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}