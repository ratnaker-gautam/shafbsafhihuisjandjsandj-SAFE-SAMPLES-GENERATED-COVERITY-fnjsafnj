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

void buffer_free(Buffer* buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
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
        
        if (count > 3 || current == 0xFF) {
            output[output_size++] = 0xFF;
            if (output_size >= output_capacity) break;
            output[output_size++] = current;
            if (output_size >= output_capacity) break;
            output[output_size++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && output_size < output_capacity; j++) {
                output[output_size++] = current;
            }
        }
        
        i += count;
    }
    
    return output_size;
}

int read_input(Buffer* input) {
    Buffer chunk;
    buffer_init(&chunk);
    
    if (!buffer_reserve(&chunk, CHUNK_SIZE)) {
        return 0;
    }
    
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(chunk.data, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) break;
        
        if (!buffer_append(input, chunk.data, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
        
        total_read += bytes_read;
        if (total_read >= MAX_INPUT_SIZE) break;
    }
    
    buffer_free(&chunk);
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
        buffer_free(&input);
        return 1;
    }
    
    size_t max_output_size = input.size * 2;
    if (max_output_size > MAX_INPUT_SIZE) {
        max_output_size = MAX_INPUT_SIZE;
    }
    
    if (!buffer_reserve(&output, max_output_size)) {
        fprintf(stderr, "Error: Failed to allocate output buffer\n");
        buffer_free(&input);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input.data, input.size, output.data, output.capacity);
    output.size = compressed_size;
    
    if (compressed_size == 0) {
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
    
    fprintf(stderr, "Compressed %zu bytes to %zu bytes\n", input.size, output.size);
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}