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
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity <= buf->capacity) abort();
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
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
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 1;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, current);
            buffer_append(output, (unsigned char)count);
        } else {
            for (size_t j = 0; j < count; j++) {
                buffer_append(output, current);
            }
        }
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char current = *(input + i);
        
        if (current == 0xFF && i + 2 < input_size) {
            unsigned char value = *(input + i + 1);
            unsigned char count = *(input + i + 2);
            
            for (int j = 0; j < count; j++) {
                buffer_append(output, value);
            }
            
            i += 3;
        } else {
            buffer_append(output, current);
            i++;
        }
    }
    
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        if (c < 0 || c > 255) continue;
        *(input_data + input_size) = (unsigned char)c;
        input_size++;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    Buffer compressed;
    buffer_init(&compressed, input_size);
    
    if (!compress_rle(input_data, input_size, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100.0);
    
    Buffer decompressed;
    buffer_init(&decompressed, compressed.size * 2);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_size || 
        memcmp(decompressed.data, input_data, input_size) != 0) {
        printf("Verification failed: decompressed data doesn't match original.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Verification successful: data compressed and decompressed correctly.\n");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return 0;
}