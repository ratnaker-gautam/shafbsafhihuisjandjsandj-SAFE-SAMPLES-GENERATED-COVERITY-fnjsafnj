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
    if (initial_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
    
    buf->data = malloc(initial_capacity * sizeof(uint8_t));
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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity * sizeof(uint8_t));
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    const uint8_t *input_end = input + input_size;
    uint8_t *output_ptr = output;
    uint8_t *output_end = output + output_capacity;
    
    while (input < input_end) {
        uint8_t current = *input;
        size_t count = 1;
        
        const uint8_t *next = input + 1;
        while (next < input_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (output_ptr + 2 > output_end) return 0;
        
        *output_ptr++ = current;
        *output_ptr++ = (uint8_t)count;
        
        input += count;
    }
    
    return output_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    const uint8_t *input_end = input + input_size;
    uint8_t *output_ptr = output;
    uint8_t *output_end = output + output_capacity;
    
    while (input < input_end) {
        uint8_t value = *input++;
        uint8_t count = *input++;
        
        if (output_ptr + count > output_end) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            *output_ptr++ = value;
        }
    }
    
    return output_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&compressed_buf, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize compressed buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    if (!buffer_init(&decompressed_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize decompressed buffer\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        return 1;
    }
    
    printf("Enter text to compress (max %zu characters): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (