//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 64;
    buf->data = malloc(initial_capacity);
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (len == 0) return;
    if (buf->size > SIZE_MAX - len) exit(EXIT_FAILURE);
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) exit(EXIT_FAILURE);
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size = new_size;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0) return 0;
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) return 0;
        
        *out_ptr++ = current;
        *out_ptr++ = (uint8_t)count;
        
        in_ptr += count;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0 || input_size % 2 != 0) return 0;
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end) {
        if (in_ptr + 2 > in_end) return 0;
        
        uint8_t value = *in_ptr++;
        uint8_t count = *in_ptr++;
        
        if (out_ptr + count > out_end) return 0;
        
        uint8_t *fill_ptr = out_ptr;
        uint8_t *fill_end = out_ptr + count;
        while (fill_ptr < fill_end) {
            *fill_ptr++ = value;
        }
        out_ptr += count;
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&compressed_buf, MAX_OUTPUT_SIZE);
    buffer_init(&decompressed_buf, MAX_INPUT_SIZE);
    
    printf("Enter text to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    uint8_t temp_buf[256];
    size_t total_read = 0;
    
    while (total_read < MAX_INPUT_SIZE) {
        if (!fgets((char*)temp_buf, sizeof(temp_buf), stdin)) break;
        
        size_t len = strlen((char*)temp_buf);
        if (len == 0) break;
        
        buffer_append(&input_buf, temp_buf, len);
        total_read += len;
        
        if (temp_buf[len - 1] == '\n') break;
    }
    
    if (input_buf.size == 0) {
        printf("No input received.\n");
        free(input_buf.data);
        free(compressed_buf.data);
        free(decompressed_buf.data);
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed_buf.data, compressed_buf.capacity);
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        free(input_buf.data);
        free(compressed_buf.data);
        free(decompressed_buf.data);
        return EXIT_FAILURE;
    }
    compressed_buf.size = compressed_size;
    
    size_t decompressed_size = decompress_rle(compressed_buf.data, compressed_buf.size, decompressed_buf.data, decompressed_b