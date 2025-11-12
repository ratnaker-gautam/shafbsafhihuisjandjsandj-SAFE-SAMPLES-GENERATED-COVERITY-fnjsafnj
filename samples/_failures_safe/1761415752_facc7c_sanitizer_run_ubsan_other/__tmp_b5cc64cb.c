//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len == 0) return 1;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input_size == 0 || output_capacity == 0) return 0;
    size_t out_pos = 0;
    size_t in_pos = 0;
    while (in_pos < input_size && out_pos < output_capacity - 2) {
        uint8_t current = input[in_pos];
        size_t count = 1;
        while (in_pos + count < input_size && count < 255 && input[in_pos + count] == current) {
            count++;
        }
        if (count > 1) {
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
            in_pos += count;
        } else {
            output[out_pos++] = current;
            in_pos++;
        }
    }
    return out_pos;
}

int read_input(Buffer *input) {
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, chunk, bytes_read)) {
            return 0;
        }
        if (input->size > MAX_INPUT_SIZE) {
            return 0;
        }
    }
    if (ferror(stdin)) return 0;
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
    size_t max_output_size = input.size * 2;
    if (max_output_size > SIZE_MAX / 2) {
        fprintf(stderr, "Input too large\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    if (!buffer_append(&output, NULL, max_output_size)) {
        fprintf(stderr, "Output buffer allocation failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    size_t compressed_size = simple_compress(input.data, input.size, output.data, output.capacity);
    if (compressed_size == 0 && input.size > 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    size_t written = fwrite(output.data, 1, compressed_size, stdout);
    if (written != compressed_size) {
        fprintf(stderr, "Output write failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}