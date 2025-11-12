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
    if (capacity <= buf->capacity) return 1;
    
    size_t new_capacity = buf->capacity ? buf->capacity * 2 : CHUNK_SIZE;
    if (new_capacity < capacity) new_capacity = capacity;
    if (new_capacity > MAX_INPUT_SIZE) new_capacity = MAX_INPUT_SIZE;
    
    uint8_t* new_data = realloc(buf->data, new_capacity);
    if (!new_data) return 0;
    
    buf->data = new_data;
    buf->capacity = new_capacity;
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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (!input || !output || input_size == 0) return 0;
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_pos + 2 > input_size) return 0;
        
        output[output_pos++] = (uint8_t)count;
        output[output_pos++] = current;
        i += count;
    }
    
    return output_pos;
}

int read_input(Buffer* input) {
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(input, chunk, bytes_read)) {
            return 0;
        }
    }
    
    if (ferror(stdin)) return 0;
    return 1;
}

int main(void) {
    Buffer input, compressed;
    buffer_init(&input);
    buffer_init(&compressed);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input);
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        return 1;
    }
    
    size_t max_output_size = input.size * 2;
    if (!buffer_reserve(&compressed, max_output_size)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input.data, input.size, compressed.data);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&compressed);
        return 1;
    }
    
    compressed.size = compressed_size;
    
    size_t written = fwrite(compressed.data, 1, compressed.size, stdout);
    if (written != compressed.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input);
        buffer_free(&compressed);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&compressed);
    return 0;
}