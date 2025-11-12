//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 64

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

typedef struct {
    uint8_t data[CHUNK_SIZE];
    size_t size;
} Chunk;

Buffer* buffer_create(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > 1048576) {
        return NULL;
    }
    
    Buffer *buf = malloc(sizeof(Buffer));
    if (!buf) return NULL;
    
    buf->data = malloc(initial_capacity);
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    
    buf->size = 0;
    buf->capacity = initial_capacity;
    return buf;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
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

void buffer_free(Buffer *buf) {
    if (buf) {
        free(buf->data);
        free(buf);
    }
}

int compress_chunk(const Chunk *input, Chunk *output) {
    if (!input || !output || input->size == 0 || input->size > CHUNK_SIZE) {
        return 0;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size && out_idx < CHUNK_SIZE; ) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > CHUNK_SIZE) return 0;
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
    return 1;
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
        printf("No input provided\n");
        return 1;
    }
    
    Buffer *compressed = buffer_create(256);
    if (!compressed) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < input_len) {
        Chunk input_chunk, output_chunk;
        size_t chunk_size = input_len - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        memcpy(input_chunk.data, (uint8_t*)input_str + processed, chunk_size);
        input_chunk.size = chunk_size;
        
        if (!compress_chunk(&input_chunk, &output_chunk)) {
            fprintf(stderr, "Compression failed\n");
            buffer_free(compressed);
            return 1;
        }
        
        if (!buffer_append(compressed, output_chunk.data, output_chunk.size)) {
            fprintf(stderr, "Buffer append failed\n");
            buffer_free(compressed);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed->size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed->size / input_len * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02x", compressed->data[i]);
    }
    printf("\n");
    
    buffer_free(compressed);
    return 0;
}