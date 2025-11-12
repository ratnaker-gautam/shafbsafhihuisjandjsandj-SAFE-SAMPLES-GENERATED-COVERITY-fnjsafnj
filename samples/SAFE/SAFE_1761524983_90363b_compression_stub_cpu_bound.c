//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
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
            if (new_capacity > MAX_INPUT_SIZE) new_capacity = MAX_INPUT_SIZE;
            uint8_t* new_data = realloc(buffer.data, new_capacity);
            if (!new_data) break;
            buffer.data = new_data;
            capacity = new_capacity;
        }
        
        size_t to_read = capacity - total_read;
        size_t bytes_read = fread(buffer.data + total_read, 1, to_read, stdin);
        if (bytes_read == 0) break;
        total_read += bytes_read;
    }
    
    buffer.size = total_read;
    return buffer;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input->size == 0) {
        output->data = NULL;
        output->size = 0;
        return;
    }
    
    output->data = malloc(input->size * 2);
    if (!output->data) {
        output->size = 0;
        return;
    }
    
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
        
        if (out_pos + 2 > input->size * 2) break;
        
        output->data[out_pos++] = (uint8_t)count;
        output->data[out_pos++] = current;
        in_pos += count;
    }
    
    output->size = out_pos;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data && input.size > 0) {
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    simple_compress(&input, &compressed);
    
    if (!compressed.data && input.size > 0) {
        free(input.data);
        return EXIT_FAILURE;
    }
    
    size_t written = fwrite(compressed.data, 1, compressed.size, stdout);
    if (written != compressed.size) {
        free(input.data);
        free(compressed.data);
        return EXIT_FAILURE;
    }
    
    free(input.data);
    free(compressed.data);
    return EXIT_SUCCESS;
}