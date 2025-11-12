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
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
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
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / 2) return 0;
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
    
    uint8_t stack_buffer[CHUNK_SIZE];
    size_t stack_pos = 0;
    
    for (size_t i = 0; i < input_size; i++) {
        if (stack_pos < CHUNK_SIZE) {
            stack_buffer[stack_pos++] = input[i];
        }
        
        if (stack_pos == CHUNK_SIZE || i == input_size - 1) {
            uint8_t run_length = 1;
            for (size_t j = 1; j < stack_pos && run_length < UINT8_MAX; j++) {
                if (stack_buffer[j] == stack_buffer[0]) {
                    run_length++;
                } else {
                    break;
                }
            }
            
            if (run_length > 3) {
                uint8_t compressed[2] = {0xFF, run_length};
                if (!buffer_append(output, compressed, 2)) return;
                if (!buffer_append(output, &stack_buffer[0], 1)) return;
                memmove(stack_buffer, stack_buffer + run_length, stack_pos - run_length);
                stack_pos -= run_length;
            } else if (stack_pos == CHUNK_SIZE) {
                if (!buffer_append(output, &stack_pos, 1)) return;
                if (!buffer_append(output, stack_buffer, stack_pos)) return;
                stack_pos = 0;
            }
        }
    }
    
    if (stack_pos > 0) {
        if (!buffer_append(output, &stack_pos, 1)) return;
        if (!buffer_append(output, stack_buffer, stack_pos)) return;
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_len)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    simple_compress((uint8_t*)input_str, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.size / input_len) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    buffer_free(&compressed);
    return 0;
}