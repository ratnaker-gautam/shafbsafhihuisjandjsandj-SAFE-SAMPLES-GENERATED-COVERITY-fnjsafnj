//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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

void buffer_append(Buffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) exit(EXIT_FAILURE);
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = byte;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 1) {
            buffer_append(output, current);
            buffer_append(output, (unsigned char)count);
            i += count;
        } else {
            buffer_append(output, current);
            buffer_append(output, 1);
            i++;
        }
        
        if (output->size > MAX_OUTPUT_SIZE) return 0;
    }
    
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || input_size % 2 != 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char value = *(input + i);
        unsigned char count = *(input + i + 1);
        
        if (count == 0) return 0;
        
        for (unsigned char j = 0; j < count; j++) {
            buffer_append(output, value);
            if (output->size > MAX_OUTPUT_SIZE) return 0;
        }
        
        i += 2;
    }
    
    return 1;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    Buffer compressed, decompressed;
    
    buffer_init(&compressed, 64);
    buffer_init(&decompressed, 64);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    if (!fgets((char*)input, sizeof(input), stdin)) {
        printf("Input error\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input);
    if (input_len > 0 && *(input + input_len - 1) == '\n') {
        *(input + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_rle(input, input_len, &compressed)) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input, input_len) != 0) {
        printf("Decompression verification failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Decompressed: %s\n", decompressed.data);
    printf("Decompression successful\n");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return EXIT_SUCCESS;
}