//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
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

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len == 0) return 1;
    if (buf->size > SIZE_MAX - len) return 0;
    
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity < CHUNK_SIZE) new_capacity = CHUNK_SIZE;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int read_input(Buffer *input) {
    Buffer chunk;
    buffer_init(&chunk);
    
    uint8_t temp[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(temp, 1, CHUNK_SIZE, stdin)) > 0) {
        if (input->size + bytes_read > MAX_INPUT_SIZE) {
            buffer_free(&chunk);
            return 0;
        }
        if (!buffer_append(input, temp, bytes_read)) {
            buffer_free(&chunk);
            return 0;
        }
    }
    
    if (ferror(stdin)) {
        buffer_free(&chunk);
        return 0;
    }
    
    return 1;
}

void run_length_encode(const Buffer *input, Buffer *output) {
    if (input->size == 0) return;
    
    uint8_t current = input->data[0];
    size_t count = 1;
    
    for (size_t i = 1; i <= input->size; i++) {
        if (i < input->size && input->data[i] == current && count < 255) {
            count++;
        } else {
            uint8_t run[2] = {current, (uint8_t)count};
            if (!buffer_append(output, run, 2)) return;
            
            if (i < input->size) {
                current = input->data[i];
                count = 1;
            }
        }
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
    
    run_length_encode(&input, &output);
    
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