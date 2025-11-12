//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.capacity = (initial_capacity > 0) ? initial_capacity : 1;
    buf.data = malloc(buf.capacity);
    if (!buf.data) {
        buf.size = 0;
        buf.capacity = 0;
        return buf;
    }
    buf.size = 0;
    return buf;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + data_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        
        uint8_t* new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void free_buffer(Buffer* buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!input || !output || !output_size || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > *output_size) return 0;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        
        in_idx += count;
    }
    
    *output_size = out_idx;
    return 1;
}

int main(void) {
    uint8_t input_stack[CHUNK_SIZE];
    Buffer compressed = create_buffer(CHUNK_SIZE);
    Buffer output_buffer = create_buffer(CHUNK_SIZE);
    
    if (!compressed.data || !output_buffer.data) {
        fprintf(stderr, "Memory allocation failed\n");
        free_buffer(&compressed);
        free_buffer(&output_buffer);
        return 1;
    }
    
    size_t total_read = 0;
    
    while (1) {
        if (total_read >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            free_buffer(&compressed);
            free_buffer(&output_buffer);
            return 1;
        }
        
        size_t bytes_read = fread(input_stack, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) break;
        
        total_read += bytes_read;
        
        uint8_t chunk_output[CHUNK_SIZE * 2];
        size_t output_size = sizeof(chunk_output);
        
        if (!compress_chunk(input_stack, bytes_read, chunk_output, &output_size)) {
            fprintf(stderr, "Compression failed\n");
            free_buffer(&compressed);
            free_buffer(&output_buffer);
            return 1;
        }
        
        if (!buffer_append(&compressed, chunk_output, output_size)) {
            fprintf(stderr, "Buffer append failed\n");
            free_buffer(&compressed);
            free_buffer(&output_buffer);
            return 1;
        }
    }
    
    uint32_t original_size = (uint32_t)total_read;
    if (fwrite(&original_size, sizeof(original_size), 1, stdout) != 1) {
        fprintf(stderr, "Write error\n");
        free_buffer(&compressed);
        free_buffer(&output_buffer);
        return 1;
    }
    
    if (fwrite(compressed.data, 1, compressed.size, stdout) != compressed.size) {
        fprintf(stderr, "Write error\n");
        free_buffer(&compressed);
        free_buffer(&output_buffer);
        return 1;
    }
    
    free_buffer(&compressed);
    free_buffer(&output_buffer);
    return 0;
}