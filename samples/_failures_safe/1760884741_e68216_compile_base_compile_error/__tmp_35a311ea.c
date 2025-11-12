//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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

Buffer* buffer_create(size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > 1048576) {
        return NULL;
    }
    
    Buffer *buf = malloc(sizeof(Buffer));
    if (!buf) {
        return NULL;
    }
    
    buf->data = malloc(initial_capacity);
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    
    buf->size = 0;
    buf->capacity = initial_capacity;
    return buf;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0) {
        return 0;
    }
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) {
            return 0;
        }
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) {
            return 0;
        }
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf) {
        free(buf->data);
        free(buf);
    }
}

int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (!input || !output || len == 0 || len > CHUNK_SIZE) {
        return 0;
    }
    
    size_t out_pos = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[out_pos++] = 0xFF;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
            i += count;
        } else {
            output[out_pos++] = current;
            i++;
        }
        
        if (out_pos >= CHUNK_SIZE * 2) {
            return 0;
        }
    }
    
    return (int)out_pos;
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[--input_len] = '\0';
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
        size_t chunk_len = input_len - processed;
        if (chunk_len > CHUNK_SIZE) {
            chunk_len = CHUNK_SIZE;
        }
        
        uint8_t chunk[CHUNK_SIZE];
        memcpy(chunk, (uint8_t*)input + processed, chunk_len);
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        int compressed_len = compress_chunk(chunk, chunk_len, compressed_chunk);
        
        if (compressed_len <= 0) {
            fprintf(stderr, "Compression failed\n");
            buffer_free(compressed);
            return 1;
        }
        
        if (!buffer_append(compressed, compressed_chunk, (size_t)compressed_len)) {
            fprintf(stderr, "Buffer append failed\n");
            buffer_free(compressed);
            return 1;
        }
        
        processed += chunk_len;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed->size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed->size) / input_len * 100.0f);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed->size; i++) {
        printf("%02X", compressed->data[i]);
    }
    printf("\n