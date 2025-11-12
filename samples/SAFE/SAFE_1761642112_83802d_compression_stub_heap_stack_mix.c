//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

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
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        if (new_capacity > SIZE_MAX / 2) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || !output || input_len == 0) return;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && input[i + count] == current && count < 255) {
            count++;
        }
        
        uint8_t chunk[2] = {current, (uint8_t)count};
        if (!buffer_append(output, chunk, 2)) return;
        
        i += count;
    }
}

int main(void) {
    char input_stack[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    if (!fgets(input_stack, sizeof(input_stack), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input_len = strlen(input_stack);
    if (input_len > 0 && input_stack[input_len - 1] == '\n') {
        input_stack[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(input_len);
    if (!compressed.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress((uint8_t*)input_stack, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x", compressed.data[i]);
        if ((i + 1) % 16 == 0 && i + 1 < compressed.size) printf("\n");
    }
    printf("\n");
    
    destroy_buffer(&compressed);
    return 0;
}