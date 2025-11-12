//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define CHUNK_SIZE 4096
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

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_len = 0;
    size_t i = 0;
    while (i < input_len && output_len + 2 <= output_capacity) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        output[output_len++] = (uint8_t)count;
        output[output_len++] = current;
        i += count;
    }
    if (i < input_len) {
        return 0;
    }
    return output_len;
}

int read_input(Buffer *buf) {
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(buf, chunk, bytes_read)) {
            return 0;
        }
    }
    if (ferror(stdin)) {
        return 0;
    }
    return 1;
}

int main(void) {
    Buffer input_buf;
    if (!buffer_init(&input_buf, CHUNK_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    if (!read_input(&input_buf)) {
        fprintf(stderr, "Input reading failed\n");
        buffer_free(&input_buf);
        return 1;
    }
    if (input_buf.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input_buf);
        return 1;
    }
    size_t max_output_size = input_buf.size * 2;
    if (max_output_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        buffer_free(&input_buf);
        return 1;
    }
    uint8_t *output_buf = malloc(max_output_size);
    if (output_buf == NULL) {
        fprintf(stderr, "Output buffer allocation failed\n");
        buffer_free(&input_buf);
        return 1;
    }
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_buf, max_output_size);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        free(output_buf);
        buffer_free(&input_buf);
        return 1;
    }
    size_t written = fwrite(output_buf, 1, compressed_size, stdout);
    if (written != compressed_size) {
        fprintf(stderr, "Output write failed\n");
        free(output_buf);
        buffer_free(&input_buf);
        return 1;
    }
    free(output_buf);
    buffer_free(&input_buf);
    return 0;
}