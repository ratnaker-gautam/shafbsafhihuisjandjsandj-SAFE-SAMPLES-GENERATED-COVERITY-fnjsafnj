//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
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

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len == 0) return 1;
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

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int read_input(Buffer *input) {
    Buffer chunk;
    buffer_init(&chunk);
    chunk.data = malloc(CHUNK_SIZE);
    if (!chunk.data) return 0;
    chunk.capacity = CHUNK_SIZE;
    
    size_t total_read = 0;
    while (1) {
        if (total_read >= MAX_INPUT_SIZE) {
            buffer_free(&chunk);
            return 0;
        }
        
        size_t to_read = CHUNK_SIZE;
        if (total_read + to_read > MAX_INPUT_SIZE) {
            to_read = MAX_INPUT_SIZE - total_read;
        }
        
        size_t bytes_read = fread(chunk.data, 1, to_read, stdin);
        if (bytes_read == 0) break;
        
        if (!buffer_append(input, chunk.data, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
        total_read += bytes_read;
    }
    
    buffer_free(&chunk);
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        uint8_t run[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run, 2)) return;
        
        i = j;
    }
}

int main(void) {
    Buffer input, output;
    buffer_init(&input);
    buffer_init(&output);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Failed to read input or input too large\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Error: No input data\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    simple_compress(input.data, input.size, &output);
    
    if (output.size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    size_t written = fwrite(output.data, 1, output.size, stdout);
    if (written != output.size) {
        fprintf(stderr, "Error: Failed to write output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}