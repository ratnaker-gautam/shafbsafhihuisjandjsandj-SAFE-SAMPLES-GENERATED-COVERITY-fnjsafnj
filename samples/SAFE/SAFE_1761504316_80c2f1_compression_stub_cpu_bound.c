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

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 64;
    buf->data = malloc(initial_capacity);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) free(buf->data);
    buf->data = NULL;
    buf->size = buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len == 0) return 1;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t run[2] = {0xFF, current};
            if (!buffer_append(output, run, 2)) return;
            uint8_t count_byte = (uint8_t)count;
            if (!buffer_append(output, &count_byte, 1)) return;
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_size) {
                if (i + 2 < input_size && input[i] == input[i + 1] && input[i] == input[i + 2]) {
                    break;
                }
                i++;
                if (i - literal_start >= 128) break;
            }
            
            size_t literal_len = i - literal_start;
            if (literal_len > 0) {
                uint8_t len_byte = (uint8_t)literal_len;
                if (!buffer_append(output, &len_byte, 1)) return;
                if (!buffer_append(output, input + literal_start, literal_len)) return;
            }
        }
    }
}

int read_input(Buffer *input) {
    uint8_t chunk[CHUNK_SIZE];
    size_t total_read = 0;
    
    while (1) {
        size_t bytes_read = fread(chunk, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            return 0;
        }
        
        total_read += bytes_read;
        if (total_read > MAX_INPUT_SIZE) return 0;
        
        if (!buffer_append(input, chunk, bytes_read)) return 0;
    }
    
    return 1;
}

int main(void) {
    Buffer input, output;
    
    if (!buffer_init(&input, 8192)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (!buffer_init(&output, 8192)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    
    if (!read_input(&input)) {
        fprintf(stderr, "Input reading failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    simple_compress(input.data, input.size, &output);
    
    if (fwrite(output.data, 1, output.size, stdout) != output.size) {
        fprintf(stderr, "Output writing failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}