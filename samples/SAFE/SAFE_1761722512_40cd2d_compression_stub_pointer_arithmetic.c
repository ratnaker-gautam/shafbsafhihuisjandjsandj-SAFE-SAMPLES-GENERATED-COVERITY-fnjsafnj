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
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(ByteBuffer *buf, const unsigned char *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity < buf->size + len) {
            if (new_capacity > SIZE_MAX / 2) abort();
            new_capacity *= 2;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
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
    if (input == NULL || output == NULL || output_capacity == 0) return 0;
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
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
    ByteBuffer input_buf, output_buf;
    buffer_init(&input_buf, BUFFER_SIZE);
    buffer_init(&output_buf, BUFFER_SIZE);
    
    unsigned char read_buf[BUFFER_SIZE];
    size_t bytes_read;
    
    printf("Enter data to compress (max %zu bytes): ", BUFFER_SIZE);
    fflush(stdout);
    
    bytes_read = fread(read_buf, 1, BUFFER_SIZE, stdin);
    if (bytes_read == 0 || ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    buffer_append(&input_buf, read_buf, bytes_read);
    
    unsigned char *compressed = malloc(input_buf.size * 2);
    if (compressed == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, input_buf.size * 2);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        free(compressed);
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return 1;
    }
    
    buffer_append(&output_buf, compressed, compressed_size);
    free(compressed);
    
    double ratio = (double)output_buf.size / input_buf.size * 100.0;
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", output_buf.size);
    printf("Compression ratio: %.2f%%\n", ratio);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output_buf.size; i++) {
        printf("%02x", output_buf.data[i]);
    }
    printf("\n");
    
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    return 0;
}