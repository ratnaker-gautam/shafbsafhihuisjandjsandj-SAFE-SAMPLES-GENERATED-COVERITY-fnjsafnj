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

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void process_chunk(Chunk *chunk, Buffer *output) {
    if (chunk == NULL || output == NULL || chunk->size == 0) return;
    
    uint8_t processed[CHUNK_SIZE * 2];
    size_t processed_size = 0;
    
    for (size_t i = 0; i < chunk->size; i++) {
        if (processed_size >= sizeof(processed)) break;
        processed[processed_size++] = chunk->data[i];
        if (i > 0 && chunk->data[i] == chunk->data[i-1]) {
            if (processed_size >= sizeof(processed)) break;
            processed[processed_size++] = chunk->data[i];
        }
    }
    
    buffer_append(output, processed, processed_size);
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len-1] == '\n') {
        input[input_len-1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_len * 2)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < input_len) {
        Chunk chunk;
        chunk.size = 0;
        
        size_t remaining = input_len - processed;
        size_t chunk_data_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        for (size_t i = 0; i < chunk_data_size; i++) {
            chunk.data[i] = (uint8_t)input[processed + i];
        }
        chunk.size = chunk_data_size;
        
        process_chunk(&chunk, &compressed);
        processed += chunk_data_size;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed.size) / input_len * 100.0f);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (compressed.size > 32) printf("...");
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}