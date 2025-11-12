//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 4096
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_reserve(Buffer *buf, size_t capacity) {
    if (capacity > MAX_INPUT_SIZE) return 0;
    if (buf->capacity >= capacity) return 1;
    
    uint8_t *new_data = realloc(buf->data, capacity);
    if (!new_data) return 0;
    
    buf->data = new_data;
    buf->capacity = capacity;
    return 1;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t size) {
    if (size > SIZE_MAX - buf->size) return 0;
    if (!buffer_reserve(buf, buf->size + size)) return 0;
    
    memcpy(buf->data + buf->size, data, size);
    buf->size += size;
    return 1;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size == 0) return 0;
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (output_pos + 2 > input_size) return 0;
            output[output_pos++] = current;
            output[output_pos++] = (uint8_t)count;
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_size && (i - literal_start < 127) && 
                   (i + 1 >= input_size || input[i] != input[i + 1] || 
                    (i + 2 >= input_size || input[i] != input[i + 2]))) {
                i++;
            }
            
            size_t literal_len = i - literal_start;
            if (output_pos + literal_len + 1 > input_size) return 0;
            
            output[output_pos++] = (uint8_t)(0x80 | literal_len);
            memcpy(output + output_pos, input + literal_start, literal_len);
            output_pos += literal_len;
        }
    }
    
    return output_pos;
}

int read_input(Buffer *buf) {
    uint8_t chunk[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin)) > 0) {
        if (!buffer_append(buf, chunk, bytes_read)) {
            return 0;
        }
        if (buf->size > MAX_INPUT_SIZE) {
            return 0;
        }
    }
    
    if (ferror(stdin)) return 0;
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
    
    if (!buffer_reserve(&output, input.size * 2)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    
    size_t compressed_size = simple_compress(input.data, input.size, output.data);
    if (compressed_size == 0 && input.size > 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (fwrite(output.data, 1, compressed_size, stdout) != compressed_size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}