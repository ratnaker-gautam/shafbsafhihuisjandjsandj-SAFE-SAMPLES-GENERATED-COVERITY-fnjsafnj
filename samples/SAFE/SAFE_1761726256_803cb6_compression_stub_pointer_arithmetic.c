//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    if (initial_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
    
    buf->data = malloc(initial_capacity * sizeof(uint8_t));
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

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL) return 0;
    if (len == 0) return 1;
    
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (new_capacity < new_size) new_capacity = new_size;
        if (new_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
        
        uint8_t *new_data = realloc(buf->data, new_capacity * sizeof(uint8_t));
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, src, len);
    buf->size = new_size;
    return 1;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_pos + 2 > output_capacity) return 0;
        
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        
        *(output + output_pos) = (uint8_t)count;
        *(output + output_pos + 1) = current;
        output_pos += 2;
        i += count;
    }
    
    return output_pos;
}

int main(void) {
    Buffer input_buf, output_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    
    if (!buffer_init(&output_buf, MAX_OUTPUT_SIZE)) {
        fprintf(stderr, "Failed to initialize output buffer\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    printf("Enter data to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (bytes_read >= MAX_INPUT_SIZE) {
            fprintf(stderr, "Input too large\n");
            buffer_free(&input_buf);
            buffer_free(&output_buf);
            return 1;
        }
        
        if (!buffer_append(&input_buf, (uint8_t*)&c, 1)) {
            fprintf(stderr, "Failed to append to input buffer\n");
            buffer_free(&input_buf);
            buffer_free(&output_buf);
            return 1;
        }
        bytes_read++;
    }
    
    if (input_buf.size == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_buf.data, output_buf.capacity);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X ", *(output_buf.data + i));
    }
    printf("\n");
    
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    return 0;
}