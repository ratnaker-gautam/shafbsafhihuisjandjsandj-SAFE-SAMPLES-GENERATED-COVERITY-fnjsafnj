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
    if (!buf || !data || len == 0 || len > SIZE_MAX - buf->size) {
        return 0;
    }
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        if (new_capacity > SIZE_MAX / 2) {
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

void free_buffer(Buffer *buf) {
    if (buf) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int compress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t *out_len) {
    if (!input || !output || !out_len || len == 0 || len > CHUNK_SIZE) {
        return 0;
    }
    
    size_t compressed = 0;
    for (size_t i = 0; i < len; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (compressed + 2 > CHUNK_SIZE * 2) {
            return 0;
        }
        
        output[compressed++] = (uint8_t)count;
        output[compressed++] = current;
        i += count;
    }
    
    *out_len = compressed;
    return 1;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t bytes_read = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while (bytes_read < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_stack[bytes_read++] = (uint8_t)c;
    }
    
    if (bytes_read == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed = create_buffer(bytes_read * 2);
    if (!compressed.data) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    size_t total_compressed = 0;
    int success = 1;
    
    for (size_t i = 0; i < bytes_read && success; i += CHUNK_SIZE) {
        size_t chunk_len = bytes_read - i;
        if (chunk_len > CHUNK_SIZE) {
            chunk_len = CHUNK_SIZE;
        }
        
        uint8_t chunk_output[CHUNK_SIZE * 2];
        size_t compressed_len;
        
        if (compress_chunk(input_stack + i, chunk_len, chunk_output, &compressed_len)) {
            if (compressed_len > 0) {
                success = buffer_append(&compressed, chunk_output, compressed_len);
                total_compressed += compressed_len;
            }
        } else {
            success = 0;
        }
    }
    
    if (!success) {
        printf("Compression failed.\n");
        free_buffer(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", bytes_read);
    printf("Compressed size: %zu bytes\n", total_compressed);
    printf("Compression ratio: %.2f%%\n", 
           (double)total_compressed / bytes_read * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < total_compressed && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (total_compressed > 32) {
        printf("...");
    }
    printf("\n");
    
    free_buffer(&compressed);
    return 0;
}