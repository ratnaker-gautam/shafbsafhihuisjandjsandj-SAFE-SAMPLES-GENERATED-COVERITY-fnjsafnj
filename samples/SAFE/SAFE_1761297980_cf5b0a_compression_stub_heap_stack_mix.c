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
    if (buf->size > SIZE_MAX - len) return -1;
    if (buf->size + len > buf->capacity) {
        size_t new_cap = buf->capacity * 2;
        if (new_cap < buf->capacity) return -1;
        if (new_cap < buf->size + len) new_cap = buf->size + len;
        uint8_t *new_data = realloc(buf->data, new_cap);
        if (new_data == NULL) return -1;
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 0;
}

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return;
    
    uint8_t stack_buf[CHUNK_SIZE];
    size_t stack_idx = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (stack_idx < CHUNK_SIZE) {
            stack_buf[stack_idx++] = input[i];
        }
        
        if (stack_idx == CHUNK_SIZE || i == input_len - 1) {
            if (buffer_append(output, stack_buf, stack_idx) != 0) {
                return;
            }
            stack_idx = 0;
        }
    }
}

int read_input(uint8_t *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return -1;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    if (fgets((char*)buffer, max_size, stdin) == NULL) return -1;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    return len;
}

int main(void) {
    uint8_t input_stack[MAX_INPUT_SIZE];
    int input_len = read_input(input_stack, MAX_INPUT_SIZE);
    
    if (input_len <= 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    Buffer compressed;
    if (buffer_init(&compressed, CHUNK_SIZE) != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress(input_stack, input_len, &compressed);
    
    printf("Original size: %d bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (compressed.size > 32) printf("...");
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}