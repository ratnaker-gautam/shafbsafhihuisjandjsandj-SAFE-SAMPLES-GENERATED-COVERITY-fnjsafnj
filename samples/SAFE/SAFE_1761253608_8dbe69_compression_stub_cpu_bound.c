//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
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

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (data_size > MAX_INPUT_SIZE - buf->size) {
        return 0;
    }
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity || new_capacity > MAX_INPUT_SIZE) {
            new_capacity = MAX_INPUT_SIZE;
        }
        if (new_capacity < buf->size + data_size) {
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
    buf->size += data_size;
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_size = 0;
    size_t i = 0;
    while (i < input_size && output_size < output_capacity - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        if (count > 2) {
            output[output_size++] = current;
            output[output_size++] = (uint8_t)count;
            i += count;
        } else {
            output[output_size++] = current;
            i++;
        }
    }
    return output_size;
}

int read_input(Buffer *input) {
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
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    size_t compressed_size = simple_compress(input.data, input.size, output.data, output.capacity);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    size_t written = fwrite(output.data, 1, compressed_size, stdout);
    if (written != compressed_size) {
        fprintf(stderr, "Output writing failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}