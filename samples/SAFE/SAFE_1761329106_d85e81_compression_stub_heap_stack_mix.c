//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf = {NULL, 0, 0};
    if (initial_capacity > 0 && initial_capacity <= SIZE_MAX / 2) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / 2) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void free_buffer(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return;
    
    for (size_t i = 0; i < input_size; i++) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + 1 < input_size && input[i + 1] == current && count < UINT8_MAX) {
            count++;
            i++;
        }
        
        uint8_t run_length_data[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run_length_data, 2)) {
            return;
        }
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (!buffer || max_size == 0) return 0;
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        
        buffer[total_read] = (uint8_t)c;
        total_read++;
    }
    
    return total_read;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    int input_size = read_input(input_stack, MAX_INPUT_SIZE);
    
    if (input_size <= 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(CHUNK_SIZE);
    if (!compressed.data) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    simple_compress(input_stack, input_size, &compressed);
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_size * 100.0f);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i += 2) {
        if (i + 1 < compressed.size) {
            printf("(%d,%d) ", compressed.data[i], compressed.data[i + 1]);
        }
    }
    printf("\n");
    
    free_buffer(&compressed);
    return 0;
}