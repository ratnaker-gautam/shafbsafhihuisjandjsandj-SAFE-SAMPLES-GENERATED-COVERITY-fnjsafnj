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
        
        if (out_idx + 2 > CHUNK_SIZE * 2) {
            return 0;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    *output_size = out_idx;
    return 1;
}

int main(void) {
    char input_stack[MAX_INPUT_SIZE];
    Buffer compressed = create_buffer(MAX_INPUT_SIZE * 2);
    
    if (!compressed.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets(input_stack, sizeof(input_stack), stdin)) {
        fprintf(stderr, "Input reading failed\n");
        free_buffer(&compressed);
        return 1;
    }
    
    size_t input_len = strlen(input_stack);
    if (input_len > 0 && input_stack[input_len - 1] == '\n') {
        input_stack[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        free_buffer(&compressed);
        return 1;
    }
    
    printf("Original: %s\n", input_stack);
    printf("Original size: %zu bytes\n", input_len);
    
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_size = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        uint8_t output_stack[CHUNK_SIZE * 2];
        size_t compressed_size = 0;
        
        if (compress_chunk((uint8_t*)input_stack + i, chunk_size, output_stack, &compressed_size)) {
            if (!buffer_append(&compressed, output_stack, compressed_size)) {
                fprintf(stderr, "Compression buffer overflow\n");
                free_buffer(&compressed);
                return 1;
            }
        } else {
            fprintf(stderr, "Chunk compression failed\n");
            free_buffer(&compressed);
            return 1;
        }
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_len * 100.0f);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    free_buffer(&compressed);
    return 0;
}