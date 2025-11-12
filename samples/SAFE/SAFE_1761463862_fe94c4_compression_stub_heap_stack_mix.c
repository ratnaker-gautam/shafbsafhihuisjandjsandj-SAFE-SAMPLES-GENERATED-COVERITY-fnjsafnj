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
    if (initial_capacity == 0 || initial_capacity > SIZE_MAX / 2) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (!buf->data) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!data || data_size == 0) {
        return 0;
    }
    if (buf->size > SIZE_MAX - data_size) {
        return 0;
    }
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) {
                return 0;
            }
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (!input || !output || !output_size || input_size == 0) {
        *output_size = 0;
        return;
    }
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 > CHUNK_SIZE) {
                break;
            }
            output[out_idx++] = 0xFF;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
        } else {
            if (out_idx + count > CHUNK_SIZE) {
                break;
            }
            for (size_t j = 0; j < count; j++) {
                output[out_idx++] = current;
            }
        }
        i += count;
    }
    *output_size = out_idx;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_size >= MAX_INPUT_SIZE) {
            break;
        }
        input_stack[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, CHUNK_SIZE * 2)) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) > CHUNK_SIZE ? CHUNK_SIZE : (input_size - i);
        uint8_t output_stack[CHUNK_SIZE * 2];
        size_t compressed_size;
        
        compress_chunk(input_stack + i, chunk_size, output_stack, &compressed_size);
        
        if (compressed_size > 0) {
            if (!buffer_append(&compressed, output_stack, compressed_size)) {
                printf("Compression buffer overflow.\n");
                buffer_free(&compressed);
                return 1;
            }
        }
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}