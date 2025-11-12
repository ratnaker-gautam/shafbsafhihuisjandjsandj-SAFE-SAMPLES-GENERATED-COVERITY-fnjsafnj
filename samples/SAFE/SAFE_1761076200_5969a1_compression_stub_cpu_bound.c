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
    size_t capacity;
} Buffer;

void buffer_init(Buffer* buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_reserve(Buffer* buf, size_t capacity) {
    if (capacity > MAX_INPUT_SIZE) return 0;
    if (buf->capacity >= capacity) return 1;
    
    uint8_t* new_data = realloc(buf->data, capacity);
    if (!new_data) return 0;
    
    buf->data = new_data;
    buf->capacity = capacity;
    return 1;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t size) {
    if (size > MAX_INPUT_SIZE - buf->size) return 0;
    
    if (!buffer_reserve(buf, buf->size + size)) return 0;
    
    memcpy(buf->data + buf->size, data, size);
    buf->size += size;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (input_size == 0) return 0;
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[output_pos++] = 0xFF;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output[output_pos++] = current;
            }
        }
        
        i += count;
    }
    
    return output_pos;
}

int read_input(Buffer* input) {
    Buffer chunk;
    buffer_init(&chunk);
    
    if (!buffer_reserve(&chunk, CHUNK_SIZE)) {
        return 0;
    }
    
    size_t bytes_read;
    while ((bytes_read = fread(chunk.data, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, chunk.data, bytes_read)) {
            free(chunk.data);
            return 0;
        }
    }
    
    free(chunk.data);
    return 1;
}

int main(void) {
    Buffer input, output;
    buffer_init(&input);
    buffer_init(&output);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Failed to read input\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Error: No input data\n");
        free(input.data);
        return 1;
    }
    
    size_t max_output_size = input.size * 2;
    if (max_output_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Error: Input too large\n");
        free(input.data);
        return 1;
    }
    
    if (!buffer_reserve(&output, max_output_size)) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(input.data);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input.data, input.size, output.data);
    
    if (compressed_size > output.capacity) {
        fprintf(stderr, "Error: Compression overflow\n");
        free(input.data);
        free(output.data);
        return 1;
    }
    
    output.size = compressed_size;
    
    size_t written = fwrite(output.data, 1, output.size, stdout);
    if (written != output.size) {
        fprintf(stderr, "Error: Failed to write output\n");
        free(input.data);
        free(output.data);
        return 1;
    }
    
    free(input.data);
    free(output.data);
    return 0;
}