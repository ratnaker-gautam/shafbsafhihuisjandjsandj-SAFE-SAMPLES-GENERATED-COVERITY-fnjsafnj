//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define CHUNK_SIZE 512

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.capacity = (initial_capacity > 0) ? initial_capacity : 1;
    buf.data = malloc(buf.capacity);
    if (!buf.data) {
        buf.size = 0;
        buf.capacity = 0;
        return buf;
    }
    buf.size = 0;
    return buf;
}

void destroy_buffer(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0) return 0;
    
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

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0) return;
    
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t stack_pos = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (stack_pos < CHUNK_SIZE) {
            stack_buffer[stack_pos++] = input[i];
        }
        
        if (stack_pos == CHUNK_SIZE || i == input_len - 1) {
            if (!buffer_append(output, stack_buffer, stack_pos)) {
                return;
            }
            stack_pos = 0;
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
        
        buffer[total_read++] = (uint8_t)c;
    }
    
    return total_read;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    
    int input_len = read_input(input_stack, MAX_INPUT_SIZE);
    if (input_len <= 0) {
        printf("No input received\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(input_len);
    if (!compressed.data) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    simple_compress(input_stack, input_len, &compressed);
    
    printf("Original size: %d bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (compressed.size > 32) printf("...");
    printf("\n");
    
    destroy_buffer(&compressed);
    return 0;
}