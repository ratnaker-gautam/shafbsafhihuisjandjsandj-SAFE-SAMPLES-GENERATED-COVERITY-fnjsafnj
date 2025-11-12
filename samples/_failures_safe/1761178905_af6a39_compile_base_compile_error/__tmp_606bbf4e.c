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
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_reserve(ByteBuffer *buf, size_t needed) {
    if (needed > SIZE_MAX / 2) return 0;
    size_t new_capacity = buf->capacity;
    if (new_capacity == 0) new_capacity = 16;
    while (new_capacity < needed) {
        if (new_capacity > SIZE_MAX / 2) return 0;
        new_capacity *= 2;
    }
    uint8_t *new_data = realloc(buf->data, new_capacity);
    if (!new_data) return 0;
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int buffer_append(ByteBuffer *buf, const uint8_t *src, size_t len) {
    if (!buffer_reserve(buf, buf->size + len)) return 0;
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_len, ByteBuffer *output) {
    if (!input || input_len == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 1 || current >= 128) {
            if (output->size + 2 > output->capacity && !buffer_reserve(output, output->size + 2)) return 0;
            *(output->data + output->size) = 0xFF;
            *(output->data + output->size + 1) = (uint8_t)count;
            output->size += 2;
        }
        
        if (output->size + 1 > output->capacity && !buffer_reserve(output, output->size + 1)) return 0;
        *(output->data + output->size) = current;
        output->size += 1;
        
        i += count;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, ByteBuffer *output) {
    if (!input || input_len == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        if (i + 1 < input_len && *(input + i) == 0xFF) {
            uint8_t count = *(input + i + 1);
            if (count == 0) return 0;
            
            if (i + 2 >= input_len) return 0;
            uint8_t value = *(input + i + 2);
            
            if (!buffer_reserve(output, output->size + count)) return 0;
            for (uint8_t j = 0; j < count; j++) {
                *(output->data + output->size + j) = value;
            }
            output->size += count;
            i += 3;
        } else {
            if (!buffer_reserve(output, output->size + 1)) return 0;
            *(output->data + output->size) = *(input + i);
            output->size += 1;
            i += 1;
        }
    }
    
    return 1;
}

int main(void) {
    ByteBuffer input, compressed, decompressed;
    buffer_init(&input);
    buffer_init(&compressed);
    buffer_init(&decompressed);
    
    printf("Enter text to compress (max %d characters): ", BUFFER_SIZE - 1);
    char text_buffer[BUFFER_SIZE];
    if (!fgets(text_buffer, sizeof(text_buffer), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    size_t text_len = strlen(text_buffer);
    if (text_len > 0 && text_buffer[text_len - 1] == '\n') {
        text_buffer[--text_len] = '\0';
    }
    
    if (text_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!buffer_append(&input, (uint8_t*)text_buffer, text_len)) {
        printf("Memory allocation failed\n");
        buffer_free(&input);
        return 1;
    }
    
    printf("Original: %s\n", input.data);
    printf("Original size: %zu bytes\n", input.size);
    
    if (!compress_rle(input.data, input.size, &compressed)) {
        printf("Compression failed\n");
        buffer_free(&input);
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    float ratio = (compressed.size * 100.0f) / input.size;