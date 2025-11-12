//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0 || src == NULL) return;
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > MAX_OUTPUT_SIZE / 2) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    if (buf->size + len <= buf->capacity) {
        memcpy(buf->data + buf->size, src, len);
        buf->size += len;
    }
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current < end && out_ptr < out_end - 2) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) break;
        
        *out_ptr++ = value;
        *out_ptr++ = (unsigned char)count;
        
        current += count;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current < end - 1 && out_ptr < out_end) {
        unsigned char value = *current++;
        unsigned char count = *current++;
        
        if (out_ptr + count > out_end) break;
        
        for (unsigned char i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, 256);
    buffer_init(&compressed_buf, 512);
    buffer_init(&decompressed_buf, 256);
    
    unsigned char test_data[] = "AAAABBBCCDAA";
    size_t test_size = strlen((char*)test_data);
    
    if (test_size > 0 && test_size <= MAX_INPUT_SIZE) {
        buffer_append(&input_buf, test_data, test_size);
        
        unsigned char compressed[MAX_OUTPUT_SIZE];
        size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, sizeof(compressed));
        
        if (compressed_size > 0 && compressed_size <= sizeof(compressed)) {
            buffer_append(&compressed_buf, compressed, compressed_size);
            
            unsigned char decompressed[MAX_INPUT_SIZE];
            size_t decompressed_size = decompress_rle(compressed_buf.data, compressed_buf.size, decompressed, sizeof(decompressed));
            
            if (decompressed_size > 0 && decompressed_size <= sizeof(decompressed)) {
                buffer_append(&decompressed_buf, decompressed, decompressed_size);
                
                if (input_buf.size == decompressed_buf.size && 
                    memcmp(input_buf.data, decompressed_buf.data, input_buf.size) == 0) {
                    printf("Compression successful: %zu -> %zu -> %zu bytes\n