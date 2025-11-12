//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

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
    if (!buf->data) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!data || len == 0) {
        return 0;
    }
    if (buf->size > SIZE_MAX - len) {
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
        if (!new_data) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void compress_data(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) {
        return;
    }
    size_t run_length = 1;
    for (size_t i = 1; i <= input_size; i++) {
        if (i < input_size && input[i] == input[i-1] && run_length < UINT8_MAX) {
            run_length++;
        } else {
            uint8_t run_byte = input[i-1];
            uint8_t count_byte = (uint8_t)run_length;
            if (!buffer_append(output, &count_byte, 1) || !buffer_append(output, &run_byte, 1)) {
                return;
            }
            run_length = 1;
        }
    }
}

int read_input_data(Buffer *input) {
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(stack_buffer, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, stack_buffer, bytes_read)) {
            return 0;
        }
        if (input->size >= MAX_INPUT_SIZE) {
            break;
        }
    }
    if (ferror(stdin)) {
        return 0;
    }
    return 1;
}

int main(void) {
    Buffer input_data;
    Buffer compressed_data;
    
    if (!buffer_init(&input_data, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&compressed_data, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&input_data);
        return 1;
    }
    
    if (!read_input_data(&input_data)) {
        fprintf(stderr, "Error reading input data\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    if (input_data.size == 0) {
        fprintf(stderr, "No input data to compress\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    compress_data(input_data.data, input_data.size, &compressed_data);
    
    if (compressed_data.size == 0) {
        fprintf(stderr, "Compression produced no output\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    size_t written = fwrite(compressed_data.data, 1, compressed_data.size, stdout);
    if (written != compressed_data.size) {
        fprintf(stderr, "Error writing compressed data\n");
        buffer_free(&input_data);
        buffer_free(&compressed_data);
        return 1;
    }
    
    buffer_free(&input_data);
    buffer_free(&compressed_data);
    
    return 0;
}