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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
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

void run_length_encode(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input_size == 0) {
        return;
    }
    uint8_t current = input[0];
    size_t count = 1;
    for (size_t i = 1; i < input_size; i++) {
        if (input[i] == current && count < UINT8_MAX) {
            count++;
        } else {
            if (!buffer_append(output, &current, 1) || !buffer_append(output, (uint8_t*)&count, 1)) {
                return;
            }
            current = input[i];
            count = 1;
        }
    }
    if (!buffer_append(output, &current, 1) || !buffer_append(output, (uint8_t*)&count, 1)) {
        return;
    }
}

size_t read_input(Buffer *input) {
    uint8_t chunk[CHUNK_SIZE];
    size_t total_read = 0;
    while (1) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) {
                break;
            }
            return 0;
        }
        if (!buffer_append(input, chunk, bytes_read)) {
            return 0;
        }
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) {
            return 0;
        }
    }
    return total_read;
}

int main(void) {
    Buffer input_data;
    if (!buffer_init(&input_data, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    size_t input_size = read_input(&input_data);
    if (input_size == 0) {
        fprintf(stderr, "Input read failed or empty\n");
        buffer_free(&input_data);
        return 1;
    }
    Buffer compressed_data;
    if (!buffer_init(&compressed_data, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input_data);
        return 1;
    }
    run_length_encode(input_data.data, input_data.size, &compressed_data);
    size_t written = fwrite(compressed_data.data, 1, compressed_data.size, stdout);
    if (written != compressed_data.size) {
        fprintf(stderr, "Output write failed\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    buffer_free(&input_data);
    buffer_free(&compressed_data);
    return 0;
}