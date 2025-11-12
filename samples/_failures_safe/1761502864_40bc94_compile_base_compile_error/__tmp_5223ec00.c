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

void buffer_init(ByteBuffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(ByteBuffer *buf, const uint8_t *src, size_t count) {
    if (count == 0) return;
    if (buf->size > SIZE_MAX - count) abort();
    size_t new_size = buf->size + count;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) abort();
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, count);
    buf->size = new_size;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end) {
        uint8_t current = *in_ptr;
        size_t count = 1;
        
        const uint8_t *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) return 0;
        
        *out_ptr++ = current;
        *out_ptr++ = (uint8_t)count;
        
        in_ptr += count;
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    const uint8_t *in_ptr = input;
    const uint8_t *in_end = input + input_size;
    uint8_t *out_ptr = output;
    uint8_t *out_end = output + output_capacity;
    
    while (in_ptr < in_end) {
        uint8_t value = *in_ptr++;
        uint8_t count = *in_ptr++;
        
        if (out_ptr + count > out_end) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    ByteBuffer input_buf, compressed_buf, decompressed_buf;
    
    buffer_init(&input_buf, BUFFER_SIZE);
    buffer_init(&compressed_buf, BUFFER_SIZE);
    buffer_init(&decompressed_buf, BUFFER_SIZE);
    
    printf("Enter text to compress (max %d chars): ", BUFFER_SIZE - 1);
    
    char temp_buf[BUFFER_SIZE];
    if (fgets(temp_buf, sizeof(temp_buf), stdin) == NULL) {
        printf("Input error\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    size_t input_len = strlen(temp_buf);
    if (input_len > 0 && temp_buf[input_len - 1] == '\n') {
        temp_buf[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    buffer_append(&input_buf, (uint8_t*)temp_buf, input_len);
    
    uint8_t *compressed_data = malloc(input_buf.size * 2);
    if (compressed_data == NULL) abort();
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed_data, input_buf.size * 2);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        free(compressed_data);
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_b