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

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > MAX_OUTPUT_SIZE) return 0;
    
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        if (new_capacity > MAX_OUTPUT_SIZE) return 0;
        
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_end = src + len;
    
    while (src < src_end) {
        *dest = *src;
        dest++;
        src++;
    }
    
    buf->size += len;
    return 1;
}

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) return 0;
    if (input_len > MAX_INPUT_SIZE) return 0;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        unsigned char run_length = (unsigned char)count;
        if (!buffer_append(output, &run_length, 1)) return 0;
        if (!buffer_append(output, &value, 1)) return 0;
        
        current = next;
    }
    
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || input_len == 0 || output == NULL) return 0;
    if (input_len % 2 != 0) return 0;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        unsigned char count = *current;
        current++;
        
        if (current >= end) return 0;
        
        unsigned char value = *current;
        current++;
        
        for (size_t i = 0; i < count; i++) {
            if (!buffer_append(output, &value, 1)) return 0;
        }
    }
    
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter input data (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        input_data[input_len] = (unsigned char)c;
        input_len++;
    }
    
    if (input_len == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_len)) {
        printf("Failed to initialize compression buffer.\n");
        return 1;
    }
    
    if (!compress_rle(input_data, input_len, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed.size) / input_len * 100);
    
    Buffer decompressed;
    if (!buffer_init(&decompressed, input_len)) {
        printf("Failed to initialize decompression buffer.\n");
        buffer_free(&compressed);
        return 1;
    }
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size != input_len) {
        printf("De