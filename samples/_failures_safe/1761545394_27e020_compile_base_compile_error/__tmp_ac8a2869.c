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
        if (out_idx >= CHUNK_SIZE - 2) return 0;
        
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            size_t literal_start = in_idx;
            while (in_idx < input_size && (in_idx - literal_start < 127) && 
                   (in_idx + 1 >= input_size || input[in_idx] != input[in_idx + 1] || 
                    (in_idx + 2 >= input_size || input[in_idx] != input[in_idx + 2]))) {
                in_idx++;
            }
            
            size_t literal_count = in_idx - literal_start;
            if (literal_count > 0) {
                if (out_idx + literal_count + 1 >= CHUNK_SIZE) return 0;
                output[out_idx++] = 0x80 | (uint8_t)literal_count;
                memcpy(output + out_idx, input + literal_start, literal_count);
                out_idx += literal_count;
            }
        }
    }
    
    *output_size = out_idx;
    return 1;
}

int main(void) {
    uint8_t stack_buffer[CHUNK_SIZE];
    Buffer input_buf = create_buffer(CHUNK_SIZE);
    Buffer output_buf = create_buffer(CHUNK_SIZE);
    
    if (!input_buf.data || !output_buf.data) {
        fprintf(stderr, "Memory allocation failed\n");
        free_buffer(&input_buf);
        free_buffer(&output_buf);
        return 1;
    }
    
    size_t total_read = 0;
    
    while (1) {
        if (total_read >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            free_buffer(&input_buf);
            free_buffer(&output_buf);
            return 1;
        }
        
        size_t bytes_read = fread(stack_buffer, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) break;
        
        if (!buffer_append(&input_buf, stack_buffer, bytes_read)) {
            fprintf(stderr, "Buffer append failed\n");
            free_buffer(&input_buf);
            free_buffer(&output_buf);
            return 1;
        }
        
        total_read += bytes_read;
    }
    
    if (input_buf.size == 0) {
        fprintf(stderr, "No input data\n");
        free_buffer(&input_buf);
        free_buffer(&output_buf);
        return 1;
    }
    
    for (size_t i = 0; i < input_buf.size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_buf.size - i < CHUNK_SIZE) ? input_buf.size - i : CHUNK_SIZE;
        uint8_t compressed[CHUNK_SIZE * 2];
        size_t compressed_size = 0;
        
        if (!compress_chunk(input_buf.data + i, chunk_size, compressed, &compressed_size)) {
            fprintf(stderr, "Comp