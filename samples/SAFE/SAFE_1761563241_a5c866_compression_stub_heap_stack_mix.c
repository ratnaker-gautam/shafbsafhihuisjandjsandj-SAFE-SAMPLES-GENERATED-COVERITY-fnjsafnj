//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHUNK_SIZE 64
#define MAX_INPUT_SIZE 1024

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

void free_buffer(Buffer *buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int append_to_buffer(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + data_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return;
    
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t processed = 0;
    
    while (processed < input_size) {
        size_t chunk_size = (input_size - processed < CHUNK_SIZE) ? 
                           input_size - processed : CHUNK_SIZE;
        
        for (size_t i = 0; i < chunk_size; i++) {
            stack_buffer[i] = input[processed + i] ^ 0xAA;
        }
        
        if (!append_to_buffer(output, stack_buffer, chunk_size)) {
            return;
        }
        
        processed += chunk_size;
    }
}

void simple_decompress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return;
    
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t processed = 0;
    
    while (processed < input_size) {
        size_t chunk_size = (input_size - processed < CHUNK_SIZE) ? 
                           input_size - processed : CHUNK_SIZE;
        
        for (size_t i = 0; i < chunk_size; i++) {
            stack_buffer[i] = input[processed + i] ^ 0xAA;
        }
        
        if (!append_to_buffer(output, stack_buffer, chunk_size)) {
            return;
        }
        
        processed += chunk_size;
    }
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(input_len);
    if (!compressed.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress((uint8_t*)input_str, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = create_buffer(compressed.size);
    if (!decompressed.data) {
        fprintf(stderr, "Memory allocation failed\n");
        free_buffer(&compressed);
        return 1;
    }
    
    simple_decompress(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size == input_len && 
        memcmp(decompressed.data, input_str, input_len) == 0) {
        printf("Decompression successful\n");
        printf("Decompressed text: %.*s\n", (int)decompressed.size, decompressed.data);
    } else {
        fprintf(stderr, "Decompression failed\n");
    }
    
    free_buffer(&compressed);
    free_buffer(&decompressed);
    
    return 0;
}