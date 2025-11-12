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
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) exit(EXIT_FAILURE);
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0) return 0;
    
    const uint8_t *end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *output_end = output + output_capacity;
    
    while (input < end) {
        uint8_t current = *input;
        size_t count = 1;
        
        const uint8_t *next = input + 1;
        while (next < end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > output_end) return 0;
        
        *out_ptr = current;
        out_ptr++;
        *out_ptr = (uint8_t)count;
        out_ptr++;
        
        input += count;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (!input || !output || input_size == 0 || output_capacity == 0 || input_size % 2 != 0) return 0;
    
    const uint8_t *end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *output_end = output + output_capacity;
    
    while (input < end) {
        uint8_t value = *input;
        input++;
        uint8_t count = *input;
        input++;
        
        if (out_ptr + count > output_end) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            *out_ptr = value;
            out_ptr++;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, 64);
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    int c;
    size_t bytes_read = 0;
    while ((c = getchar()) != EOF && c != '\n' && bytes_read < MAX_INPUT_SIZE) {
        if (c < 0 || c > 255) continue;
        buffer_append(&input_buf, (uint8_t)c);
        bytes_read++;
    }
    
    if (input_buf.size == 0) {
        printf("No input provided.\n");
        free(input_buf.data);
        return EXIT_FAILURE;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        free(input_buf.data);
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100.0);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress_rle(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    
    if (decompressed_size != input_buf.size || memcmp(input_buf.data, decompressed, input_buf.size) != 0) {
        printf("Decompression verification failed.\n");
        free(input_buf.data);
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully.\n");
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        putchar(decompressed[i]);
    }
    printf("\n");
    
    free(input_buf.data);
    return EXIT_SUCCESS;
}