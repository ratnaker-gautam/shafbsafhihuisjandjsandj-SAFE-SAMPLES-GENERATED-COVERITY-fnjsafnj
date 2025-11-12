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
    buf->data = malloc(initial_capacity);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL) return 0;
    if (len == 0) return 1;
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    uint8_t *dest = buf->data + buf->size;
    for (size_t i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
    buf->size += len;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && *(input + i + count) == current) {
            count++;
        }
        
        uint8_t run[2] = {current, (uint8_t)count};
        if (!buffer_append(output, run, 2)) return 0;
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0 || input_len % 2 != 0) return 0;
    
    for (size_t i = 0; i < input_len; i += 2) {
        uint8_t value = *(input + i);
        uint8_t count = *(input + i + 1);
        
        if (count == 0) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer compressed, decompressed;
    
    buffer_init(&compressed, 64);
    buffer_init(&decompressed, 64);
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        *(input + input_len) = (uint8_t)c;
        input_len++;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        putchar(*(input + i));
    }
    printf("\n");
    
    if (!compress_rle(input, input_len, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", *(compressed.data + i));
    }
    printf("\n");
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(*(decompressed.data + i));
    }
    printf("\n");
    
    if (input_len == decompressed.size && memcmp(input, decompressed.data, input_len) == 0) {
        printf("Compression/decompression successful.\n");
    } else {
        printf("Compression/decompression failed: data mismatch.\n");
    }
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}