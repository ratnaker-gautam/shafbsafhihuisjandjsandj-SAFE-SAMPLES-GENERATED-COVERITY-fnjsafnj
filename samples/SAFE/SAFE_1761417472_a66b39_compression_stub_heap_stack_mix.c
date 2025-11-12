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
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) return 0;
        
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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0) return 0;
    
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_size && output_size < output_capacity - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[output_size++] = current;
            output[output_size++] = (uint8_t)count;
            i += count;
        } else {
            output[output_size++] = current;
            i++;
        }
    }
    
    return output_size;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer compressed_buffer = create_buffer(CHUNK_SIZE);
    Buffer output_buffer = create_buffer(CHUNK_SIZE * 2);
    
    if (!compressed_buffer.data || !output_buffer.data) {
        fprintf(stderr, "Memory allocation failed\n");
        free_buffer(&compressed_buffer);
        free_buffer(&output_buffer);
        return 1;
    }
    
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(stack_input, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) break;
        
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            free_buffer(&compressed_buffer);
            free_buffer(&output_buffer);
            return 1;
        }
        
        if (!buffer_append(&output_buffer, stack_input, bytes_read)) {
            fprintf(stderr, "Buffer append failed\n");
            free_buffer(&compressed_buffer);
            free_buffer(&output_buffer);
            return 1;
        }
    }
    
    if (output_buffer.size == 0) {
        fprintf(stderr, "No input data\n");
        free_buffer(&compressed_buffer);
        free_buffer(&output_buffer);
        return 1;
    }
    
    uint8_t* compressed_data = malloc(output_buffer.size * 2);
    if (!compressed_data) {
        fprintf(stderr, "Compression buffer allocation failed\n");
        free_buffer(&compressed_buffer);
        free_buffer(&output_buffer);
        return 1;
    }
    
    size_t compressed_size = simple_compress(output_buffer.data, output_buffer.size, compressed_data, output_buffer.size * 2);
    
    if (compressed_size > 0) {
        if (fwrite(compressed_data, 1, compressed_size, stdout) != compressed_size) {
            fprintf(stderr, "Output write failed\n");
        }
    } else {
        fprintf(stderr, "Compression failed\n");
    }
    
    free(compressed_data);
    free_buffer(&compressed_buffer);
    free_buffer(&output_buffer);
    
    return 0;
}