//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_INPUT_SIZE) {
        return 0;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        return 0;
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (data_size > SIZE_MAX - buf->size) {
        return 0;
    }
    size_t new_size = buf->size + data_size;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (new_capacity > MAX_INPUT_SIZE) {
            return 0;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    size_t output_size = 0;
    size_t i = 0;
    while (i < input_size && output_size + 2 <= output_capacity) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        output[output_size++] = (uint8_t)count;
        output[output_size++] = current;
        i += count;
    }
    if (i < input_size) {
        return 0;
    }
    return output_size;
}

int main(void) {
    uint8_t stack_input[CHUNK_SIZE];
    Buffer heap_buffer;
    Buffer compressed_buffer;
    
    if (!buffer_init(&heap_buffer, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&compressed_buffer, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&heap_buffer);
        return 1;
    }
    
    printf("Enter data to compress (max %d bytes, Ctrl+D to end):\n", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        size_t bytes_read = fread(stack_input, 1, CHUNK_SIZE, stdin);
        if (bytes_read == 0) {
            if (feof(stdin)) {
                break;
            } else {
                fprintf(stderr, "Error reading input\n");
                buffer_free(&heap_buffer);
                buffer_free(&compressed_buffer);
                return 1;
            }
        }
        
        if (!buffer_append(&heap_buffer, stack_input, bytes_read)) {
            fprintf(stderr, "Input too large or memory error\n");
            buffer_free(&heap_buffer);
            buffer_free(&compressed_buffer);
            return 1;
        }
        total_read += bytes_read;
    }
    
    if (heap_buffer.size == 0) {
        printf("No input data provided\n");
        buffer_free(&heap_buffer);
        buffer_free(&compressed_buffer);
        return 1;
    }
    
    uint8_t *compressed_data = malloc(heap_buffer.size * 2);
    if (compressed_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&heap_buffer);
        buffer_free(&compressed_buffer);
        return 1;
    }
    
    size_t compressed_size = compress_rle(heap_buffer.data, heap_buffer.size, compressed_data, heap_buffer.size * 2);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        free(compressed_data);
        buffer_free(&heap_buffer);
        buffer_free(&compressed_buffer);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", heap_buffer.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (double)compressed_size / heap_buffer.size * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02x ", compressed_data