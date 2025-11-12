//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define BUFFER_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} ByteBuffer;

void buffer_init(ByteBuffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(ByteBuffer *buf, const unsigned char *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + len) {
            if (new_capacity > SIZE_MAX / 2) exit(EXIT_FAILURE);
            new_capacity *= 2;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    size_t output_len = 0;
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;
        
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (output_len + 2 > output_capacity) return 0;
        
        *(output + output_len) = (unsigned char)count;
        *(output + output_len + 1) = value;
        output_len += 2;
        
        current = next;
    }
    
    return output_len;
}

size_t decompress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0 || input_len % 2 != 0) return 0;
    
    size_t output_len = 0;
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        unsigned char count = *current;
        unsigned char value = *(current + 1);
        
        if (output_len + count > output_capacity) return 0;
        
        unsigned char *target = output + output_len;
        for (size_t i = 0; i < count; i++) {
            *(target + i) = value;
        }
        output_len += count;
        
        current += 2;
    }
    
    return output_len;
}

int main(void) {
    ByteBuffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, BUFFER_SIZE);
    buffer_init(&compressed_buf, BUFFER_SIZE);
    buffer_init(&decompressed_buf, BUFFER_SIZE);
    
    printf("Enter text to compress (max %d characters):\n", BUFFER_SIZE - 1);
    
    unsigned char temp[BUFFER_SIZE];
    if (fgets((char*)temp, sizeof(temp), stdin) == NULL) {
        printf("Input error\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)temp);
    if (input_len > 0 && temp[input_len - 1] == '\n') {
        temp[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    buffer_append(&input_buf, temp, input_len);
    
    unsigned char compressed[BUFFER_SIZE * 2];
    size_t compressed_len = compress_rle(input_buf.data, input_buf.size, compressed, sizeof(compressed));
    
    if (compressed_len == 0) {
        printf("Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    buffer_append(&compressed_buf, compressed, compressed_len);
    
    unsigned char decompressed[BUFFER_SIZE];
    size_t decompressed_len = decompress_rle(compressed_buf.data, compressed_buf.size,