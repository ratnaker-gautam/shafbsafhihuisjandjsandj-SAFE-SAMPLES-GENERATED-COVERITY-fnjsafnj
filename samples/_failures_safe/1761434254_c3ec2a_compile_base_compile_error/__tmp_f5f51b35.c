//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

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

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (buf->size + len > new_capacity) {
            new_capacity = buf->size + len;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    size_t output_len = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end && output_len + 2 <= output_capacity) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        *(output + output_len) = (uint8_t)count;
        *(output + output_len + 1) = value;
        output_len += 2;
        
        current = next;
    }
    
    return output_len;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    size_t output_len = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current + 1 < end && output_len < output_capacity) {
        uint8_t count = *current;
        uint8_t value = *(current + 1);
        
        if (output_len + count > output_capacity) {
            count = output_capacity - output_len;
        }
        
        uint8_t *dest = output + output_len;
        for (size_t i = 0; i < count; i++) {
            *(dest + i) = value;
        }
        output_len += count;
        current += 2;
    }
    
    return output_len;
}

int main() {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE) ||
        !buffer_init(&compressed_buf, MAX_OUTPUT_SIZE) ||
        !buffer_init(&decompressed_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Input reading failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    if (!buffer_append(&input_buf, (uint8_t*)input_line, input_len)) {
        fprintf(stderr, "Input buffer append failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    size_t compressed_len =