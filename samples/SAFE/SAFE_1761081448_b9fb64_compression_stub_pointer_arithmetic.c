//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
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
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(ByteBuffer *buf, const unsigned char *src, size_t len) {
    if (len == 0) return;
    if (buf->size > SIZE_MAX - len) abort();
    size_t new_size = buf->size + len;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) abort();
            new_capacity *= 2;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size = new_size;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || output_capacity == 0) return 0;
    if (input_len == 0) return 0;
    
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    
    while (in_ptr < in_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        
        const unsigned char *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (out_ptr + 2 > out_end) return 0;
        
        *out_ptr++ = current;
        *out_ptr++ = (unsigned char)count;
        
        in_ptr += count;
    }
    
    return out_ptr - output;
}

int main(void) {
    ByteBuffer input_buf;
    buffer_init(&input_buf, BUFFER_SIZE);
    
    printf("Enter data to compress (max %d bytes): ", BUFFER_SIZE - 1);
    
    unsigned char read_buf[BUFFER_SIZE];
    if (fgets((char*)read_buf, sizeof(read_buf), stdin) == NULL) {
        printf("Error reading input\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    size_t input_len = strlen((char*)read_buf);
    if (input_len > 0 && read_buf[input_len - 1] == '\n') {
        read_buf[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    buffer_append(&input_buf, read_buf, input_len);
    
    size_t max_output_size = input_buf.size * 2;
    if (max_output_size < input_buf.size) {
        printf("Input too large\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    unsigned char *output_buf = malloc(max_output_size);
    if (output_buf == NULL) {
        printf("Memory allocation failed\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output_buf, max_output_size);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        free(output_buf);
        buffer_free(&input_buf);
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02x ", output_buf[i]);
    }
    printf("\n");
    
    free(output_buf);
    buffer_free(&input_buf);
    
    return 0;
}