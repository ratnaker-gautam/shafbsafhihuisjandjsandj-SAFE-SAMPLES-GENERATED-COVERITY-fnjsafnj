//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define CHUNK_SIZE 128

typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer create_buffer(size_t initial_capacity) {
    Buffer buf;
    buf.data = NULL;
    buf.size = 0;
    buf.capacity = 0;
    
    if (initial_capacity > 0 && initial_capacity <= SIZE_MAX / 2) {
        buf.data = malloc(initial_capacity);
        if (buf.data) {
            buf.capacity = initial_capacity;
        }
    }
    return buf;
}

int buffer_append(Buffer* buf, const uint8_t* data, size_t data_size) {
    if (!buf || !data || data_size == 0 || data_size > SIZE_MAX - buf->size) {
        return 0;
    }
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > SIZE_MAX / 2) {
            return 0;
        }
        
        uint8_t* new_data = realloc(buf->data, new_capacity);
        if (!new_data) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

void free_buffer(Buffer* buf) {
    if (buf && buf->data) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int compress_chunk(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!input || !output || !output_size || input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            if (out_idx + 2 > CHUNK_SIZE) {
                return 0;
            }
            output[out_idx++] = 0xFF;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
        } else {
            if (out_idx + count > CHUNK_SIZE) {
                return 0;
            }
            for (size_t j = 0; j < count; j++) {
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    *output_size = out_idx;
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
    
    Buffer compressed = create_buffer(bytes_read);
    if (!compressed.data) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    size_t processed = 0;
    while (processed < bytes_read) {
        size_t chunk_size = bytes_read - processed;
        if (chunk_size > CHUNK_SIZE) {
            chunk_size = CHUNK_SIZE;
        }
        
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_size = 0;
        
        if (!compress_chunk(input_stack + processed, chunk_size, compressed_chunk, &compressed_size)) {
            printf("Compression failed.\n");
            free_buffer(&compressed);
            return 1;
        }
        
        if (!buffer_append(&compressed, compressed_chunk, compressed_size)) {
            printf("Buffer append failed.\n");
            free_buffer(&compressed);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    printf("Original size: %zu bytes\n", bytes_read);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (float)compressed.size / bytes_read * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size && i < 64; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 64) {
        printf("...");
    }
    printf("\n");
    
    free_buffer(&compressed);
    return 0;
}