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
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) {
            return 0;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            new_capacity = MAX_INPUT_SIZE;
        }
        if (new_size > new_capacity) {
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

void run_length_encode(const Buffer *input, Buffer *output) {
    if (input->size == 0) {
        return;
    }
    uint8_t current = input->data[0];
    size_t count = 1;
    for (size_t i = 1; i <= input->size; i++) {
        if (i < input->size && input->data[i] == current && count < 255) {
            count++;
        } else {
            uint8_t pair[2] = {current, (uint8_t)count};
            buffer_append(output, pair, 2);
            if (i < input->size) {
                current = input->data[i];
                count = 1;
            }
        }
    }
}

int read_input(Buffer *buf) {
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
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) {
            return 0;
        }
        if (!buffer_append(buf, chunk, bytes_read)) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    Buffer input, output;
    if (!buffer_init(&input, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    if (!buffer_init(&output, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&input);
        return 1;
    }
    if (!read_input(&input)) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    run_length_encode(&input, &output);
    fwrite(output.data, 1, output.size, stdout);
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}