//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

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

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf->data == NULL || data == NULL || len == 0) {
        return 0;
    }
    if (len > SIZE_MAX - buf->size) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t *out_len) {
    if (input == NULL || output == NULL || out_len == NULL || len == 0) {
        *out_len = 0;
        return;
    }
    size_t out_idx = 0;
    size_t i = 0;
    while (i < len) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        if (count > 3 || current == 0xFF) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[out_idx++] = current;
            }
        }
        i += count;
        if (out_idx > len * 2) {
            break;
        }
    }
    *out_len = out_idx;
}

int read_input(Buffer *buf) {
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(buf, chunk, bytes_read)) {
            return 0;
        }
        if (buf->size >= MAX_INPUT_SIZE) {
            break;
        }
    }
    if (ferror(stdin)) {
        return 0;
    }
    return 1;
}

void process_compression(Buffer *input, Buffer *output) {
    if (input->size == 0) {
        return;
    }
    size_t max_output_size = input->size * 2 + 1024;
    if (!buffer_init(output, max_output_size)) {
        return;
    }
    size_t chunk_count = (input->size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    for (size_t i = 0; i < chunk_count; i++) {
        size_t chunk_start = i * CHUNK_SIZE;
        size_t chunk_len = CHUNK_SIZE;
        if (chunk_start + chunk_len > input->size) {
            chunk_len = input->size - chunk_start;
        }
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_len;
        compress_chunk(input->data + chunk_start, chunk_len, compressed, &compressed_len);
        if (compressed_len > 0) {
            buffer_append(output, compressed, compressed_len);
        }
    }
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    if (!buffer_init(&input, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    if (!read_input(&input)) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input);
        return 1;
    }
    process_compression(&input, &output);
    if (output.data != NULL && output.size > 0) {
        fwrite(output.data, 1, output.size, stdout);
    }
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}