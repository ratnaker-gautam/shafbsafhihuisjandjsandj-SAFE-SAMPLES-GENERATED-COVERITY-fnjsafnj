//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) {
            new_capacity = buf->size + data_size;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    uint8_t *output_ptr = output;
    uint8_t *output_end = output + output_capacity;
    const uint8_t *input_ptr = input;
    const uint8_t *input_end = input + input_size;
    
    while (input_ptr < input_end && output_ptr + 2 <= output_end) {
        uint8_t current = *input_ptr;
        size_t count = 1;
        
        const uint8_t *next_ptr = input_ptr + 1;
        while (next_ptr < input_end && *next_ptr == current && count < 255) {
            count++;
            next_ptr++;
        }
        
        if (output_ptr + 2 > output_end) break;
        
        *output_ptr++ = current;
        *output_ptr++ = (uint8_t)count;
        
        input_ptr += count;
    }
    
    return output_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    uint8_t *output_ptr = output;
    uint8_t *output_end = output + output_capacity;
    const uint8_t *input_ptr = input;
    const uint8_t *input_end = input + input_size;
    
    while (input_ptr + 1 < input_end && output_ptr < output_end) {
        uint8_t value = *input_ptr++;
        uint8_t count = *input_ptr++;
        
        if (output_ptr + count > output_end) break;
        
        for (uint8_t i = 0; i < count; i++) {
            *output_ptr++ = value;
        }
    }
    
    return output_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE) ||
        !buffer_init(&compressed_buf, MAX_OUTPUT_SIZE) ||
        !buffer_init(&decompressed_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    while (bytes_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        if (bytes_read < input_buf.capacity) {
            input_buf.data[bytes_read++] = (uint8_t)c;
        }
    }
    input_buf.size = bytes_read;
    
    if (input_buf.size == 0) {
        printf("No input data provided\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, 
                                        compressed_buf.data, compressed_buf.capacity);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1