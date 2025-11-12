//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, unsigned char value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity <= buf->capacity) exit(EXIT_FAILURE);
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (!input || !output || output_capacity == 0) return 0;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) return 0;
        
        *out_ptr = (unsigned char)count;
        out_ptr++;
        *out_ptr = value;
        out_ptr++;
        
        current += count;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (!input || !output || output_capacity == 0 || input_size % 2 != 0) return 0;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current < end) {
        unsigned char count = *current;
        current++;
        unsigned char value = *current;
        current++;
        
        if (out_ptr + count > out_end) return 0;
        
        for (size_t i = 0; i < count; i++) {
            *out_ptr = value;
            out_ptr++;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    int c;
    size_t bytes_read = 0;
    while ((c = getchar()) != EOF && c != '\n' && bytes_read < MAX_INPUT_SIZE) {
        if (bytes_read >= input_buf.capacity) {
            size_t new_capacity = input_buf.capacity * 2;
            if (new_capacity <= input_buf.capacity || new_capacity > MAX_INPUT_SIZE * 2) break;
            unsigned char *new_data = realloc(input_buf.data, new_capacity);
            if (!new_data) break;
            input_buf.data = new_data;
            input_buf.capacity = new_capacity;
        }
        *(input_buf.data + bytes_read) = (unsigned char)c;
        bytes_read++;
    }
    input_buf.size = bytes_read;
    
    if (input_buf.size == 0) {
        printf("No input provided.\n");
        free(input_buf.data);
        return EXIT_FAILURE;
    }
    
    unsigned char compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        free(input_buf.data);
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100.0);
    
    unsigned char decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    
    if (decompressed_size != input_buf.size || memcmp(input_buf.data, decompressed, input_buf.size) != 0) {
        printf("Decompression verification failed.\n");
        free(input_buf.data);
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully.\n");
    
    printf("Compressed data: ");
    for (size_t i =