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
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
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
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t marker = 0xFF;
            uint8_t count_byte = (uint8_t)count;
            uint8_t data_byte = current;
            buffer_append(output, &marker, 1);
            buffer_append(output, &count_byte, 1);
            buffer_append(output, &data_byte, 1);
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (input[i + j] == 0xFF) {
                    uint8_t escape = 0xFE;
                    buffer_append(output, &escape, 1);
                }
                buffer_append(output, &input[i + j], 1);
            }
            i += count;
        }
    }
}

int read_input(Buffer *input) {
    Buffer chunk;
    if (!buffer_init(&chunk, CHUNK_SIZE)) return 0;
    
    size_t total_read = 0;
    while (1) {
        size_t to_read = CHUNK_SIZE;
        if (total_read + to_read > MAX_INPUT_SIZE) {
            to_read = MAX_INPUT_SIZE - total_read;
            if (to_read == 0) break;
        }
        
        size_t bytes_read = fread(chunk.data, 1, to_read, stdin);
        if (bytes_read == 0) break;
        
        if (!buffer_append(input, chunk.data, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
        
        total_read += bytes_read;
        if (bytes_read < to_read || total_read >= MAX_INPUT_SIZE) break;
    }
    
    buffer_free(&chunk);
    return 1;
}

int main(void) {
    Buffer input, output;
    
    if (!buffer_init(&input, CHUNK_SIZE)) {
        fprintf(stderr, "Error: Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&output, CHUNK_SIZE)) {
        fprintf(stderr, "Error: Failed to initialize output buffer\n");
        buffer_free(&input);
        return 1;
    }
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error: Failed to read input\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Error: No input data\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    simple_compress(input.data, input.size, &output);
    
    if (fwrite(output.data, 1, output.size, stdout) != output.size) {
        fprintf(stderr, "Error: Failed to write output\n");
        buffer_free(&input);
        buffer_free(&output);
        return 1;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return 0;
}