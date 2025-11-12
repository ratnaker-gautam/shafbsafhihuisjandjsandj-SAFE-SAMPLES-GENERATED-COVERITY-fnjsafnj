//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) return 0;
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
    if (!data || len == 0) return 1;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > MAX_INPUT_SIZE) return 0;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

int read_input(Buffer *input) {
    if (!buffer_init(input, CHUNK_SIZE)) return 0;
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, chunk, bytes_read)) {
            buffer_free(input);
            return 0;
        }
        if (input->size >= MAX_INPUT_SIZE) break;
    }
    if (ferror(stdin)) {
        buffer_free(input);
        return 0;
    }
    return 1;
}

int compress_data(const Buffer *input, Buffer *output) {
    if (!input || !output || input->size == 0) return 0;
    if (!buffer_init(output, input->size)) return 0;
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        if (count >= 3) {
            uint8_t run[2] = {0xFF, current};
            if (!buffer_append(output, run, 2)) {
                buffer_free(output);
                return 0;
            }
            uint8_t count_byte = (uint8_t)count;
            if (!buffer_append(output, &count_byte, 1)) {
                buffer_free(output);
                return 0;
            }
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (input->data[i + j] == 0xFF) {
                    uint8_t escape[2] = {0xFF, 0x00};
                    if (!buffer_append(output, escape, 2)) {
                        buffer_free(output);
                        return 0;
                    }
                } else {
                    if (!buffer_append(output, &input->data[i + j], 1)) {
                        buffer_free(output);
                        return 0;
                    }
                }
            }
            i += count;
        }
    }
    return 1;
}

int write_output(const Buffer *output) {
    if (!output || output->size == 0) return 0;
    size_t written = fwrite(output->data, 1, output->size, stdout);
    if (written != output->size) return 0;
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        return 1;
    }
    if (!compress_data(&input, &output)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        return 1;
    }
    if (!write_output(&output)) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}