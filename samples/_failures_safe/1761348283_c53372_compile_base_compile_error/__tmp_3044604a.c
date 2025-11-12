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
    if (initial_capacity == 0) initial_capacity = 64;
    buf->data = malloc(initial_capacity);
    if (!buf->data) exit(1);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > SIZE_MAX / 2) exit(1);
            new_capacity *= 2;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(1);
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

int compress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (!input || input_len == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && *(input + i + count) == current) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            unsigned char marker = 0xFF;
            unsigned char count_byte = (unsigned char)count;
            buffer_append(output, &marker, 1);
            buffer_append(output, &current, 1);
            buffer_append(output, &count_byte, 1);
        } else {
            for (size_t j = 0; j < count; j++) {
                unsigned char single = *(input + i + j);
                if (single == 0xFF) {
                    unsigned char escape = 0xFE;
                    buffer_append(output, &escape, 1);
                }
                buffer_append(output, &single, 1);
            }
        }
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len, Buffer *output) {
    if (!input || input_len == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        unsigned char current = *(input + i);
        
        if (current == 0xFF && i + 2 < input_len) {
            unsigned char value = *(input + i + 1);
            unsigned char count = *(input + i + 2);
            for (unsigned int j = 0; j < count; j++) {
                buffer_append(output, &value, 1);
            }
            i += 3;
        } else if (current == 0xFE && i + 1 < input_len) {
            unsigned char value = *(input + i + 1);
            buffer_append(output, &value, 1);
            i += 2;
        } else {
            buffer_append(output, &current, 1);
            i += 1;
        }
    }
    
    return 1;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE) {
        *(input_data + input_len) = (unsigned char)c;
        input_len++;
    }
    
    if (input_len == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    Buffer compressed;
    buffer_init(&compressed, 64);
    
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
    buffer_init(&decompressed, 64);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size == input_len && 
        memcmp(decompressed.data, input_data, input_len) ==