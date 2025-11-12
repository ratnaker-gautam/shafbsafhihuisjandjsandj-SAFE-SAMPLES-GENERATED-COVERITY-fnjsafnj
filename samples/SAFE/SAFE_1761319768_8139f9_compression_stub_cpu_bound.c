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
    uint8_t* new_data = realloc(buf->data, capacity);
    if (new_data == NULL) return 0;
    buf->data = new_data;
    buf->capacity = capacity;
    return 1;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t size) {
    if (size > SIZE_MAX - buf->size) return 0;
    size_t new_size = buf->size + size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (!buffer_reserve(buf, new_capacity)) return 0;
    }
    memcpy(buf->data + buf->size, data, size);
    buf->size = new_size;
    return 1;
}

void buffer_free(Buffer* buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int read_input(Buffer* input) {
    Buffer chunk;
    buffer_init(&chunk);
    
    if (!buffer_reserve(&chunk, CHUNK_SIZE)) {
        return 0;
    }
    
    size_t total_read = 0;
    while (1) {
        if (total_read > MAX_INPUT_SIZE) {
            buffer_free(&chunk);
            return 0;
        }
        
        size_t bytes_read = fread(chunk.data, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) break;
            buffer_free(&chunk);
            return 0;
        }
        
        if (!buffer_append(input, chunk.data, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
        
        total_read += bytes_read;
    }
    
    buffer_free(&chunk);
    return 1;
}

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        uint8_t run_length = (uint8_t)count;
        if (!buffer_append(output, &run_length, 1) || !buffer_append(output, &current, 1)) {
            return;
        }
        
        i += count;
    }
}

int main(void) {
    Buffer input, output;
    buffer_init(&input);
    buffer_init(&output);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input);
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        buffer_free(&input);
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    if (!buffer_reserve(&output, input.size * 2)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input);
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    simple_compress(input.data, input.size, &output);
    
    if (fwrite(output.data, 1, output.size, stdout) != output.size) {
        fprintf(stderr, "Error writing output\n");
        buffer_free(&input);
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    buffer_free(&input);
    buffer_free(&output);
    return EXIT_SUCCESS;
}