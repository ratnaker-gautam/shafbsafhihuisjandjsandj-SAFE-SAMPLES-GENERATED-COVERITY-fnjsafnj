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

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0) return 0;
    
    if (buf->size > SIZE_MAX - len) return 0;
    
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / 2) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, len);
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

int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (!input || !output || len == 0 || len > CHUNK_SIZE) return 0;
    
    size_t out_pos = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_pos + 2 > CHUNK_SIZE * 2) return 0;
        
        output[out_pos++] = (uint8_t)count;
        output[out_pos++] = current;
        i += count;
    }
    
    return (int)out_pos;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t total_read = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while (total_read < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_stack[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(total_read * 2);
    if (!compressed.data) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < total_read) {
        size_t chunk_len = total_read - processed;
        if (chunk_len > CHUNK_SIZE) chunk_len = CHUNK_SIZE;
        
        uint8_t chunk[CHUNK_SIZE];
        memcpy(chunk, input_stack + processed, chunk_len);
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        int compressed_len = compress_chunk(chunk, chunk_len, compressed_chunk);
        
        if (compressed_len <= 0 || !buffer_append(&compressed, compressed_chunk, compressed_len)) {
            printf("Compression failed.\n");
            free_buffer(&compressed);
            return 1;
        }
        
        processed += chunk_len;
    }
    
    printf("Original size: %zu bytes\n", total_read);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (total_read > 0) ? (100.0 * compressed.size / total_read) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
    }
    printf("\n");
    
    free_buffer(&compressed);
    return 0;
}