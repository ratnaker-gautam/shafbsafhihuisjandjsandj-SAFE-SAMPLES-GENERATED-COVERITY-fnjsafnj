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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0 || output_size == 0) return 0;
    
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
            size_t literal_start = in_pos;
            size_t literal_count = 0;
            
            while (in_pos < input_size && literal_count < 255 && 
                   (in_pos + 1 >= input_size || input[in_pos] != input[in_pos + 1] || 
                    (in_pos + 2 < input_size && input[in_pos + 1] != input[in_pos + 2]))) {
                in_pos++;
                literal_count++;
            }
            
            if (out_pos + literal_count + 1 >= output_size) break;
            
            output[out_pos++] = 0xFF;
            output[out_pos++] = (uint8_t)literal_count;
            for (size_t i = 0; i < literal_count && out_pos < output_size; i++) {
                output[out_pos++] = input[literal_start + i];
            }
        }
    }
    
    return out_pos;
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    buffer_init(&input);
    buffer_init(&output);
    
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(&input, chunk, bytes_read)) {
            fprintf(stderr, "Input too large\n");
            buffer_free(&input);
            buffer_free(&output);
            return 1;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Read error\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (!buffer_reserve(&output, input.size * 2)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input.data, input.size, output.data, output.capacity);
    
    if (compressed_size > 0) {
        size_t written = fwrite(output.data, 1, compressed_size, stdout);
        if (written != compressed_size) {
            fprintf(stderr, "Write error\n");
            buffer_free(&input);
            buffer_free(&output);
            return 1;
        }
    }
    
    fprintf(stderr, "Original: %zu bytes, Compressed: %zu bytes\n", input.size, compressed_size);
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}