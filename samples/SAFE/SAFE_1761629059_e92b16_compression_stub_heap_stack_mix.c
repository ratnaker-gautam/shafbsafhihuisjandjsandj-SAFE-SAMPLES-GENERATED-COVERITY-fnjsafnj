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

Buffer* create_buffer(size_t initial_capacity) {
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

void destroy_buffer(Buffer *buf) {
    if (buf) {
        free(buf->data);
        free(buf);
    }
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

void process_chunk(Chunk *chunk, Buffer *output) {
    if (!chunk || !output || chunk->size == 0) return;
    
    uint8_t processed[CHUNK_SIZE * 2];
    size_t processed_size = 0;
    
    for (size_t i = 0; i < chunk->size; i++) {
        if (processed_size >= sizeof(processed)) break;
        
        uint8_t current = chunk->data[i];
        uint8_t count = 1;
        
        while (i + 1 < chunk->size && chunk->data[i + 1] == current && count < 255) {
            count++;
            i++;
        }
        
        processed[processed_size++] = count;
        processed[processed_size++] = current;
    }
    
    if (processed_size > 0) {
        buffer_append(output, processed, processed_size);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer *compressed = create_buffer(256);
    if (!compressed) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t total_processed = 0;
    while (total_processed < input_len) {
        Chunk chunk;
        chunk.size = 0;
        
        size_t remaining = input_len - total_processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        for (size_t i = 0; i < chunk_size; i++) {
            chunk.data[i] = (uint8_t)input[total_processed + i];
        }
        chunk.size = chunk_size;
        
        process_chunk(&chunk, compressed);
        total_processed += chunk_size;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed->size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed->size / input_len) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02x", compressed->data[i]);
        if (i < compressed->size - 1) printf(" ");
    }
    printf("\n");
    
    destroy_buffer(compressed);
    return 0;
}