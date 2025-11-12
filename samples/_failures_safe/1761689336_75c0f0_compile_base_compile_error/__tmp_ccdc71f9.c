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

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (!input || !output || output_len < 2) return 0;
    
    const uint8_t *end = input + input_len;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_len - 2;
    
    while (input < end && out_ptr < out_end) {
        uint8_t current = *input;
        size_t count = 1;
        
        const uint8_t *next = input + 1;
        while (next < end && *next == current && count < UINT8_MAX) {
            count++;
            next++;
        }
        
        *out_ptr++ = (uint8_t)count;
        *out_ptr++ = current;
        
        input += count;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (!input || !output || input_len % 2 != 0) return 0;
    
    const uint8_t *end = input + input_len;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_len;
    
    while (input < end && out_ptr < out_end) {
        uint8_t count = *input++;
        uint8_t value = *input++;
        
        if (out_ptr + count > out_end) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    ByteBuffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf);
    buffer_init(&compressed_buf);
    buffer_init(&decompressed_buf);
    
    printf("Enter text to compress (max %d chars): ", BUFFER_SIZE - 1);
    
    uint8_t temp_buf[BUFFER_SIZE];
    if (!fgets((char*)temp_buf, BUFFER_SIZE, stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)temp_buf);
    if (input_len > 0 && temp_buf[input_len - 1] == '\n') {
        temp_buf[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        buffer_append(&input_buf, temp_buf[i]);
    }
    
    uint8_t compressed[BUFFER_SIZE * 2];
    size_t compressed_len = compress_rle(input_buf.data, input_buf.size, compressed, sizeof(compressed));
    
    if (compressed_len == 0) {
        printf("Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    for (size_t i = 0; i < compressed_len; i++) {
        buffer_append(&compressed_buf, compressed[i]);
    }
    
    uint8_t decompressed[BUFFER_SIZE];
    size_t decompressed_len = decompress_rle(compressed_buf.data, compressed_buf.size, decompressed, sizeof(decompressed));
    
    if (decompressed_len == 0 || decompressed_len != input_buf.size) {
        printf("Decompression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    for (size_t i = 0; i < decompressed_len; i++) {
        buffer_append(&decompressed_buf, decompressed[i]);
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_buf.size);
    if (input_buf.size > 0) {
        double ratio = (1.0 - (double)compressed_buf.size / (