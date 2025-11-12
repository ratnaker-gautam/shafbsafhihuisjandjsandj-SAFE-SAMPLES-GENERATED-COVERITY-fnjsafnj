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
    if (len > MAX_INPUT_SIZE - buf->size) {
        return 0;
    }
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
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
    buf->size += len;
    return 1;
}

int compress_data(const Buffer *input, Buffer *output) {
    if (input->size == 0 || output == NULL) {
        return 0;
    }
    if (!buffer_init(output, input->size / 2 + 1)) {
        return 0;
    }
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        if (count >= 3) {
            uint8_t run_length = (uint8_t)count;
            if (!buffer_append(output, (uint8_t[]){0xFF, current, run_length}, 3)) {
                buffer_free(output);
                return 0;
            }
            i += count;
        } else {
            if (!buffer_append(output, &current, 1)) {
                buffer_free(output);
                return 0;
            }
            i++;
        }
    }
    return 1;
}

int read_input(Buffer *buf) {
    if (!buffer_init(buf, CHUNK_SIZE)) {
        return 0;
    }
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(buf, chunk, bytes_read)) {
            buffer_free(buf);
            return 0;
        }
        if (buf->size >= MAX_INPUT_SIZE) {
            break;
        }
    }
    if (ferror(stdin)) {
        buffer_free(buf);
        return 0;
    }
    return 1;
}

int write_output(const Buffer *buf) {
    if (buf->size == 0) {
        return 0;
    }
    size_t written = fwrite(buf->data, 1, buf->size, stdout);
    if (written != buf->size) {
        return 0;
    }
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