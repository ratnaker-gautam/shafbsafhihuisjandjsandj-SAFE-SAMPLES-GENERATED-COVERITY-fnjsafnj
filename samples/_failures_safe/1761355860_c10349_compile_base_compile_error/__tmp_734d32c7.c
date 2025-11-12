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

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
        return -1;
    }
    
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return -1;
    }
    
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
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
    if (buf == NULL || data == NULL || data_size == 0) {
        return -1;
    }
    
    if (buf->size > SIZE_MAX - data_size) {
        return -1;
    }
    
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                return -1;
            }
            new_capacity *= 2;
        }
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return -1;
        }
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 0;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    uint8_t current_byte = input[0];
    uint8_t count = 1;
    
    for (size_t i = 1; i < input_size; i++) {
        if (input[i] == current_byte && count < UINT8_MAX) {
            count++;
        } else {
            uint8_t chunk[2] = {count, current_byte};
            if (buffer_append(output, chunk, 2) != 0) {
                return;
            }
            current_byte = input[i];
            count = 1;
        }
    }
    
    uint8_t final_chunk[2] = {count, current_byte};
    buffer_append(output, final_chunk, 2);
}

void simple_decompress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t byte = input[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (buffer_append(output, &byte, 1) != 0) {
                return;
            }
        }
    }
}

int main(void) {
    char input_stack[MAX_INPUT_SIZE];
    Buffer compressed_heap, decompressed_heap;
    
    if (buffer_init(&compressed_heap, CHUNK_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize compression buffer\n");
        return 1;
    }
    
    if (buffer_init(&decompressed_heap, CHUNK_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize decompression buffer\n");
        buffer_free(&compressed_heap);
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_stack, sizeof(input_stack), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&compressed_heap);
        buffer_free(&decompressed_heap);
        return 1;
    }
    
    size_t input_len = strlen(input_stack);
    if (input_len > 0 && input_stack[input_len - 1] == '\n') {
        input_stack[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&compressed_heap);
        buffer_free(&decompressed_heap);
        return 1;
    }
    
    simple_compress((uint8_t*)input_stack, input_len, &compressed_heap);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_heap.size);
    printf("Compression ratio: %.2f%%\n", 
           (compressed_heap.size * 100.0) / input_len);
    
    simple_decompress(compressed_heap.data, compressed_heap.size, &decompressed_heap);
    
    if (decompressed_heap.size == input_len &&