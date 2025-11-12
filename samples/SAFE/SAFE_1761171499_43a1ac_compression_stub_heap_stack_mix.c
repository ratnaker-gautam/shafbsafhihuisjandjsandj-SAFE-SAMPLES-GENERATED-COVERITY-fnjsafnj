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
    if (initial_capacity == 0 || initial_capacity > 65536) {
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
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void compress_chunk(const Chunk *input, Chunk *output) {
    if (!input || !output) return;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        if (output->size + 2 > CHUNK_SIZE) break;
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
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
        printf("No input provided\n");
        return 1;
    }
    
    Buffer *compressed = create_buffer(256);
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
        
        compress_chunk(&input_chunk, &output_chunk);
        
        if (!buffer_append(compressed, output_chunk.data, output_chunk.size)) {
            fprintf(stderr, "Compression buffer overflow\n");
            destroy_buffer(compressed);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed->size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed->size) / input_len * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02x", compressed->data[i]);
        if (i < compressed->size - 1) printf(" ");
    }
    printf("\n");
    
    destroy_buffer(compressed);
    return 0;
}