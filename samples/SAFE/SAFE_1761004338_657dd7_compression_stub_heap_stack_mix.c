//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: compression_stub
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
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf == NULL || data == NULL || len == 0) return 0;
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void compress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t *out_len) {
    if (input == NULL || output == NULL || out_len == NULL || len == 0) {
        *out_len = 0;
        return;
    }
    size_t j = 0;
    for (size_t i = 0; i < len && j < len - 1; ) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < len && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        if (count > 2) {
            output[j++] = current;
            output[j++] = (uint8_t)count;
            i += count;
        } else {
            output[j++] = current;
            i++;
        }
    }
    *out_len = j;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    Buffer compressed_buffer;
    
    if (!buffer_init(&compressed_buffer, CHUNK_SIZE)) {
        fprintf(stderr, "Failed to initialize buffer\n");
        return 1;
    }
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char*)input_stack, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&compressed_buffer);
        return 1;
    }
    
    size_t input_len = strlen((char*)input_stack);
    if (input_len > 0 && input_stack[input_len - 1] == '\n') {
        input_stack[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&compressed_buffer);
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : input_len - i;
        uint8_t compressed_chunk[CHUNK_SIZE * 2];
        size_t compressed_len;
        
        compress_chunk(input_stack + i, chunk_len, compressed_chunk, &compressed_len);
        
        if (!buffer_append(&compressed_buffer, compressed_chunk, compressed_len)) {
            fprintf(stderr, "Failed to append compressed data\n");
            buffer_free(&compressed_buffer);
            return 1;
        }
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_buffer.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed_buffer.size / input_len) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_buffer.size; i++) {
        printf("%02X ", compressed_buffer.data[i]);
    }
    printf("\n");
    
    buffer_free(&compressed_buffer);
    return 0;
}