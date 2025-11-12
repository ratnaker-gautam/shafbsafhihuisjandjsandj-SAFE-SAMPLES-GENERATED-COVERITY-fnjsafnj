//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 1024

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
    *(buf->data + buf->size) = byte;
    buf->size++;
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
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 1) {
            buffer_append(output, current);
            buffer_append(output, (uint8_t)count);
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_len && *(input + i) != *(input + i + 1) && (i - literal_start) < UINT8_MAX) {
                i++;
            }
            size_t literal_len = i - literal_start;
            if (literal_len == 0) literal_len = 1;
            
            buffer_append(output, 0xFF);
            buffer_append(output, (uint8_t)literal_len);
            for (size_t j = 0; j < literal_len; j++) {
                buffer_append(output, *(input + literal_start + j));
            }
        }
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, ByteBuffer *output) {
    if (!input || !output || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t flag = *(input + i);
        i++;
        
        if (i >= input_len) return 0;
        uint8_t count = *(input + i);
        i++;
        
        if (flag == 0xFF) {
            if (i + count > input_len) return 0;
            for (uint8_t j = 0; j < count; j++) {
                buffer_append(output, *(input + i + j));
            }
            i += count;
        } else {
            for (uint8_t j = 0; j < count; j++) {
                buffer_append(output, flag);
            }
        }
    }
    return 1;
}

int main(void) {
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5, 6, 6, 7, 8, 8, 8};
    size_t test_len = sizeof(test_data) / sizeof(test_data[0]);
    
    ByteBuffer compressed;
    buffer_init(&compressed);
    
    if (!compress_rle(test_data, test_len, &compressed)) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Original size: %zu\n", test_len);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / test_len) * 100);
    
    ByteBuffer decompressed;
    buffer_init(&decompressed);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    if (decompressed.size != test_len) {
        printf("Size mismatch: %zu vs %zu\n", decompressed.size, test_len);
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < test_len; i++) {
        if (*(test_data + i) != *(decompressed.data + i)) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression verified: data matches original\n");
    } else {
        printf("Decompression failed: data mismatch\n");
    }
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    return 0;
}