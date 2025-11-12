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
    if (buf == NULL || initial_capacity == 0) return -1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return -1;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 0;
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
    if (buf == NULL || data == NULL || len == 0) return -1;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 0;
}

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        while (i + count < input_len && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        uint8_t run_length = (uint8_t)count;
        if (buffer_append(output, &run_length, 1) != 0) return;
        if (buffer_append(output, &current, 1) != 0) return;
        
        i += count;
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return -1;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    if (fgets((char*)buffer, max_size, stdin) == NULL) return -1;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len;
}

void print_compressed(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL || buf->size == 0) return;
    
    printf("Compressed data (%zu bytes): ", buf->size);
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    Buffer output_heap;
    
    if (buffer_init(&output_heap, CHUNK_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        return 1;
    }
    
    int input_len = read_input(input_stack, MAX_INPUT_SIZE);
    if (input_len <= 0) {
        fprintf(stderr, "Failed to read input or input too large\n");
        buffer_free(&output_heap);
        return 1;
    }
    
    simple_compress(input_stack, input_len, &output_heap);
    
    if (output_heap.size > 0) {
        printf("Original size: %d bytes\n", input_len);
        printf("Compressed size: %zu bytes\n", output_heap.size);
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)output_heap.size / input_len) * 100);
        print_compressed(&output_heap);
    } else {
        printf("Compression produced no output\n");
    }
    
    buffer_free(&output_heap);
    return 0;
}