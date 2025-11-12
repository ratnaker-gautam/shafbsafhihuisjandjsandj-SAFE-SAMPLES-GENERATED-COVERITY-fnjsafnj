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

int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (len == 0) {
        return 1;
    }
    if (buf->size > SIZE_MAX - len) {
        return 0;
    }
    size_t new_size = buf->size + len;
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
    memcpy(buf->data + buf->size, data, len);
    buf->size = new_size;
    return 1;
}

void simple_compress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input_len == 0) {
        return;
    }
    uint8_t stack_buf[CHUNK_SIZE];
    size_t stack_idx = 0;
    uint8_t current = input[0];
    uint8_t count = 1;
    
    for (size_t i = 1; i < input_len; i++) {
        if (input[i] == current && count < UINT8_MAX) {
            count++;
        } else {
            if (stack_idx + 2 > CHUNK_SIZE) {
                if (!buffer_append(output, stack_buf, stack_idx)) {
                    return;
                }
                stack_idx = 0;
            }
            stack_buf[stack_idx++] = count;
            stack_buf[stack_idx++] = current;
            current = input[i];
            count = 1;
        }
    }
    if (stack_idx + 2 > CHUNK_SIZE) {
        if (!buffer_append(output, stack_buf, stack_idx)) {
            return;
        }
        stack_idx = 0;
    }
    stack_buf[stack_idx++] = count;
    stack_buf[stack_idx++] = current;
    if (stack_idx > 0) {
        buffer_append(output, stack_buf, stack_idx);
    }
}

void simple_decompress(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input_len == 0 || input_len % 2 != 0) {
        return;
    }
    uint8_t stack_buf[CHUNK_SIZE];
    size_t stack_idx = 0;
    
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (stack_idx >= CHUNK_SIZE) {
                if (!buffer_append(output, stack_buf, stack_idx)) {
                    return;
                }
                stack_idx = 0;
            }
            stack_buf[stack_idx++] = value;
        }
    }
    if (stack_idx > 0) {
        buffer_append(output, stack_buf, stack_idx);
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Buffer compressed, decompressed;
    if (!buffer_init(&compressed, 64) || !buffer_init(&decompressed, 64)) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    simple_compress((uint8_t*)input_str, input_len, &compressed);
    simple_decompress(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size != input_len || memcmp(decompressed.data, input_str, input_len) !=