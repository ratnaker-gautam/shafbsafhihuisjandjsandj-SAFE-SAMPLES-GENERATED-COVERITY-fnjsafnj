//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
    
    buf->data = malloc(initial_capacity * sizeof(uint8_t));
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
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (count >= 3) {
            uint8_t run_length = (count > UINT8_MAX) ? UINT8_MAX : count;
            uint8_t compressed_chunk[2] = {0xFF, run_length};
            if (!buffer_append(output, compressed_chunk, 2)) return;
            if (!buffer_append(output, &current, 1)) return;
            i += run_length;
        } else {
            for (size_t j = 0; j < count; j++) {
                uint8_t single_byte = input[i + j];
                if (single_byte != 0xFF) {
                    if (!buffer_append(output, &single_byte, 1)) return;
                } else {
                    uint8_t escape_seq[2] = {0xFF, 0x01};
                    if (!buffer_append(output, escape_seq, 2)) return;
                }
            }
            i += count;
        }
    }
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while (input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_stack[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE)) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    simple_compress(input_stack, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 32) printf("...");
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}