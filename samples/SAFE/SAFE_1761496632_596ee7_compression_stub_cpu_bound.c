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

void buffer_init(Buffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_reserve(Buffer *buf, size_t needed) {
    if (needed > SIZE_MAX / 2) return 0;
    size_t new_capacity = buf->capacity;
    if (new_capacity == 0) new_capacity = 64;
    while (new_capacity < needed) {
        if (new_capacity > SIZE_MAX / 2) return 0;
        new_capacity *= 2;
    }
    uint8_t *new_data = realloc(buf->data, new_capacity);
    if (!new_data) return 0;
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len > SIZE_MAX - buf->size) return 0;
    if (!buffer_reserve(buf, buf->size + len)) return 0;
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int read_input(Buffer *input) {
    Buffer chunk;
    buffer_init(&chunk);
    
    if (!buffer_reserve(&chunk, CHUNK_SIZE)) {
        buffer_free(&chunk);
        return 0;
    }
    
    size_t total_read = 0;
    while (1) {
        if (total_read > MAX_INPUT_SIZE) {
            buffer_free(&chunk);
            return 0;
        }
        
        size_t to_read = CHUNK_SIZE;
        if (to_read > MAX_INPUT_SIZE - total_read) {
            to_read = MAX_INPUT_SIZE - total_read;
        }
        
        size_t bytes_read = fread(chunk.data, 1, to_read, stdin);
        if (bytes_read == 0) break;
        
        if (!buffer_append(input, chunk.data, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
        
        total_read += bytes_read;
        if (bytes_read < to_read) break;
    }
    
    buffer_free(&chunk);
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
    if (input->size == 0) return;
    
    size_t run_length = 1;
    uint8_t current = input->data[0];
    
    for (size_t i = 1; i < input->size; i++) {
        if (input->data[i] == current && run_length < 255) {
            run_length++;
        } else {
            uint8_t run_byte = (uint8_t)run_length;
            if (!buffer_append(output, &run_byte, 1) || !buffer_append(output, &current, 1)) {
                return;
            }
            current = input->data[i];
            run_length = 1;
        }
    }
    
    uint8_t run_byte = (uint8_t)run_length;
    if (!buffer_append(output, &run_byte, 1) || !buffer_append(output, &current, 1)) {
        return;
    }
}

int main(void) {
    Buffer input, output;
    buffer_init(&input);
    buffer_init(&output);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
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
    
    simple_compress(&input, &output);
    
    if (output.size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    size_t written = fwrite(output.data, 1, output.size, stdout);
    if (written != output.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}