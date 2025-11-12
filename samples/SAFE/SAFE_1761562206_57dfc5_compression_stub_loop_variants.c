//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} ByteBuffer;

void buffer_init(ByteBuffer *buf) {
    buf->capacity = 64;
    buf->data = malloc(buf->capacity);
    buf->size = 0;
}

void buffer_append(ByteBuffer *buf, uint8_t byte) {
    if (buf->size >= buf->capacity) {
        size_t new_cap = buf->capacity * 2;
        if (new_cap < buf->capacity) return;
        uint8_t *new_data = realloc(buf->data, new_cap);
        if (!new_data) return;
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    buf->data[buf->size++] = byte;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_len, ByteBuffer *output) {
    if (!input || !output || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        buffer_append(output, count);
        buffer_append(output, current);
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, ByteBuffer *output) {
    if (!input || !output || input_len == 0 || (input_len % 2) != 0) return 0;
    
    size_t i = 0;
    for (; i < input_len; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        size_t j = 0;
        do {
            buffer_append(output, value);
            j++;
        } while (j < count);
    }
    
    return 1;
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    ByteBuffer compressed, decompressed;
    buffer_init(&compressed);
    buffer_init(&decompressed);
    
    if (!compress_rle((uint8_t*)input_str, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100.0);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_len || 
        memcmp(decompressed.data, input_str, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Decompressed successfully: ");
    size_t k = 0;
    while (k < decompressed.size) {
        putchar(decompressed.data[k]);
        k++;
    }
    printf("\n");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return 0;
}