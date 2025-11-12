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
        if (new_capacity < buf->capacity) {
            return 0;
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

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_size = 0;
    size_t i = 0;
    while (i < input_size) {
        if (output_size + 2 > output_capacity) {
            return 0;
        }
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        output[output_size++] = (uint8_t)count;
        output[output_size++] = current;
        i += count;
    }
    return output_size;
}

int main(void) {
    Buffer input_buf;
    if (!buffer_init(&input_buf, 4096)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    uint8_t read_buf[4096];
    size_t total_read = 0;
    while (1) {
        size_t bytes_read = fread(read_buf, 1, sizeof(read_buf), stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) {
                break;
            } else {
                fprintf(stderr, "Read error\n");
                buffer_free(&input_buf);
                return 1;
            }
        }
        if (total_read > SIZE_MAX - bytes_read) {
            fprintf(stderr, "Input too large\n");
            buffer_free(&input_buf);
            return 1;
        }
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input exceeds maximum size\n");
            buffer_free(&input_buf);
            return 1;
        }
        if (!buffer_append(&input_buf, read_buf, bytes_read)) {
            fprintf(stderr, "Failed to append to input buffer\n");
            buffer_free(&input_buf);
            return 1;
        }
    }
    if (input_buf.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input_buf);
        return 1;
    }
    size_t max_output_size = input_buf.size * 2 + 1;
    if (max_output_size > MAX_OUTPUT_SIZE) {
        fprintf(stderr, "Output would exceed maximum size\n");
        buffer_free(&input_buf);
        return 1;
    }
    uint8_t *output_data = malloc(max_output_size);
    if (output_data == NULL) {
        fprintf(stderr, "Failed to allocate output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_data, max_output_size);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        free(output_data);
        buffer_free(&input_buf);
        return 1;
    }
    size_t written = fwrite(output_data, 1, compressed_size, stdout);
    if (written != compressed_size) {
        fprintf(stderr, "Write error\n");
        free(output_data);
        buffer_free(&input_buf);
        return 1;
    }
    free(output_data);
    buffer_free(&input_buf);
    return 0;