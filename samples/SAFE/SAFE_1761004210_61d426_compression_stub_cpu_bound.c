//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    size_t capacity = CHUNK_SIZE;
    buffer.data = malloc(capacity);
    if (!buffer.data) return buffer;
    
    size_t total_read = 0;
    while (1) {
        if (total_read == capacity) {
            if (capacity >= MAX_INPUT_SIZE) break;
            size_t new_capacity = capacity * 2;
            if (new_capacity < capacity || new_capacity > MAX_INPUT_SIZE) {
                new_capacity = MAX_INPUT_SIZE;
            }
            uint8_t* new_data = realloc(buffer.data, new_capacity);
            if (!new_data) break;
            buffer.data = new_data;
            capacity = new_capacity;
        }
        
        size_t to_read = capacity - total_read;
        if (to_read > CHUNK_SIZE) to_read = CHUNK_SIZE;
        
        size_t bytes_read = fread(buffer.data + total_read, 1, to_read, stdin);
        if (bytes_read == 0) break;
        total_read += bytes_read;
    }
    
    buffer.size = total_read;
    return buffer;
}

Buffer compress_rle(const Buffer* input) {
    Buffer output = {NULL, 0};
    if (input->size == 0) return output;
    
    size_t max_output_size = input->size * 2 + 1;
    output.data = malloc(max_output_size);
    if (!output.data) return output;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_pos + 2 > max_output_size) {
            free(output.data);
            output.data = NULL;
            return output;
        }
        
        output.data[out_pos++] = (uint8_t)count;
        output.data[out_pos++] = current;
        in_pos += count;
    }
    
    output.size = out_pos;
    return output;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        fprintf(stderr, "Error: Failed to allocate memory for input\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Error: No input data provided\n");
        free(input.data);
        return 1;
    }
    
    Buffer compressed = compress_rle(&input);
    free(input.data);
    
    if (!compressed.data) {
        fprintf(stderr, "Error: Failed to compress data\n");
        return 1;
    }
    
    size_t written = fwrite(compressed.data, 1, compressed.size, stdout);
    free(compressed.data);
    
    if (written != compressed.size) {
        fprintf(stderr, "Error: Failed to write all output\n");
        return 1;
    }
    
    return 0;
}