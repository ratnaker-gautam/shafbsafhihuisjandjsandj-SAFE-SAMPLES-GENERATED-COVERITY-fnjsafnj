//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

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
    if (capacity > SIZE_MAX / 2) return 0;
    if (buf->capacity >= capacity) return 1;
    
    size_t new_capacity = buf->capacity ? buf->capacity : 64;
    while (new_capacity < capacity) {
        if (new_capacity > SIZE_MAX / 2) return 0;
        new_capacity *= 2;
    }
    
    uint8_t* new_data = realloc(buf->data, new_capacity);
    if (!new_data) return 0;
    
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t size) {
    if (size > SIZE_MAX - buf->size) return 0;
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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (!input || !output || output_size == 0) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size && out_pos < output_size - 2) {
        uint8_t current = input[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input_size && count < 255 && input[in_pos + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
            in_pos += count;
        } else {
            output[out_pos++] = current;
            in_pos++;
        }
    }
    
    return out_pos;
}

int read_input(Buffer* input) {
    uint8_t chunk[CHUNK_SIZE];
    
    while (1) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            return 0;
        }
        
        if (input->size + bytes_read > MAX_INPUT_SIZE) return 0;
        if (!buffer_append(input, chunk, bytes_read)) return 0;
    }
    
    return 1;
}

int main(void) {
    Buffer input, output;
    buffer_init(&input);
    buffer_init(&output);
    
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
    if (!buffer_reserve(&output, max_output_size)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input.data, input.size, output.data, output.capacity);
    output.size = compressed_size;
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    size_t written = fwrite(output.data, 1, output.size, stdout);
    if (written != output.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}