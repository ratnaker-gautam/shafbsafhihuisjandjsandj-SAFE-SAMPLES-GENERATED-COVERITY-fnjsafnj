//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) new_capacity *= 2;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end && out_ptr < out_end - 2) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        *out_ptr++ = current;
        *out_ptr++ = (uint8_t)count;
        
        in_ptr += count;
    }
    
    if (in_ptr < in_end) return 0;
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    if (input_len % 2 != 0) return 0;
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_len;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end - 1 && out_ptr < out_end) {
        uint8_t value = *in_ptr++;
        uint8_t count = *in_ptr++;
        
        if (out_ptr + count > out_end) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    if (in_ptr != in_end) return 0;
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&compressed_buf, MAX_OUTPUT_SIZE);
    buffer_init(&decompressed_buf, MAX_INPUT_SIZE);
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    uint8_t temp_buf[128];
    
    while (bytes_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        
        if (bytes_read < MAX_INPUT_SIZE) {
            temp_buf[0] = (uint8_t)c;
            buffer_append(&input_buf, temp_buf, 1);
            bytes_read++;
        }
    }
    
    if (input_buf.size == 0) {
        printf("No input data provided.\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed_data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    buffer_append(&compressed_buf, compressed_data, compressed_size);
    
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress_rle(compressed_buf.data, compressed_buf.size, decompressed_data, MAX_INPUT_SIZE);
    
    if