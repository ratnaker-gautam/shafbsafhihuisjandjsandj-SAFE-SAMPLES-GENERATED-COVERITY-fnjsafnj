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
    if (!buf->data) exit(1);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) exit(1);
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(1);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size == 0) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, current);
            buffer_append(output, (uint8_t)count);
        } else {
            for (size_t j = 0; j < count; j++) {
                buffer_append(output, current);
            }
        }
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size == 0) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        if (*(input + i) == 0xFF && i + 2 < input_size) {
            uint8_t value = *(input + i + 1);
            uint8_t count = *(input + i + 2);
            
            for (uint8_t j = 0; j < count; j++) {
                buffer_append(output, value);
            }
            
            i += 3;
        } else {
            buffer_append(output, *(input + i));
            i++;
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed, decompressed;
    
    buffer_init(&compressed, 64);
    buffer_init(&decompressed, 64);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets((char*)input_data, MAX_INPUT_SIZE, stdin)) {
        printf("Input error\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        printf("Empty input\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Original: %s\n", input_data);
    printf("Original size: %zu bytes\n", input_size);
    
    if (!compress_rle(input_data, input_size, &compressed)) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_size * 100.0f);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_size || 
        memcmp(decompressed.data, input_data, input_size) != 0) {
        printf("Verification failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Decompressed: %s\n", decompressed.data);
    printf("Verification successful\n");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}