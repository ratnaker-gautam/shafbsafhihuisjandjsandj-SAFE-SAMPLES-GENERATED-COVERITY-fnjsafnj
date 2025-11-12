//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0 && initial_capacity <= MAX_INPUT_SIZE) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0 || len > MAX_INPUT_SIZE) return 0;
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        if (new_capacity > MAX_INPUT_SIZE) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void free_buffer(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (!input || !output || !output_len || input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (out_idx + 2 > *output_len) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    *output_len = out_idx;
    return 1;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer compressed = create_buffer(CHUNK_SIZE);
    Buffer decompressed = create_buffer(CHUNK_SIZE);
    
    if (!compressed.data || !decompressed.data) {
        fprintf(stderr, "Memory allocation failed\n");
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(stack_input, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) break;
        
        if (!buffer_append(&decompressed, stack_input, bytes_read)) {
            fprintf(stderr, "Input too large or memory error\n");
            free_buffer(&compressed);
            free_buffer(&decompressed);
            return 1;
        }
        
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input exceeds maximum size\n");
            free_buffer(&compressed);
            free_buffer(&decompressed);
            return 1;
        }
    }
    
    if (decompressed.size == 0) {
        fprintf(stderr, "No input data\n");
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    size_t max_output_size = decompressed.size * 2;
    if (max_output_size > MAX_INPUT_SIZE) max_output_size = MAX_INPUT_SIZE;
    
    uint8_t *temp_output = malloc(max_output_size);
    if (!temp_output) {
        fprintf(stderr, "Memory allocation failed\n");
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    size_t temp_len = max_output_size;
    if (!compress_rle(decompressed.data, decompressed.size, temp_output, &temp_len)) {
        fprintf(stderr, "Compression failed\n");
        free(temp_output);
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    if (!buffer_append(&compressed, temp_output, temp_len)) {
        fprintf(stderr, "Output buffer too small\n");
        free(temp_output);
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    free(temp_output);
    
    size_t written = fwrite(compressed.data, 1, compressed.size, stdout);
    if (written != compressed.size) {
        fprintf(stderr, "Output write error\n");
        free_buffer(&compressed);
        free_buffer(&decompressed);
        return 1;
    }
    
    free_buffer(&compressed);
    free_buffer(&decompressed);
    return 0;
}