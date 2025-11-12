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
    
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, stdin)) > 0) {
        buffer_append(&input_buf, buffer, bytes_read);
    }
    
    if (input_buf.size == 0) {
        fprintf(stderr, "Error: No input data\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    size_t max_output_size = input_buf.size * 2;
    if (max_output_size > SIZE_MAX / 2) {
        fprintf(stderr, "Error: Input too large\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    unsigned char *compressed = malloc(max_output_size);
    if (compressed == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, max_output_size);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        free(compressed);
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    if (fwrite(compressed, 1, compressed_size, stdout) != compressed_size) {
        fprintf(stderr, "Error: Write failed\n");
        free(compressed);
        buffer_free(&input_buf);
        buffer_free(&output_buf);
        return EXIT_FAILURE;
    }
    
    free(compressed);
    buffer_free(&input_buf);
    buffer_free(&output_buf);
    
    return EXIT_SUCCESS;
}