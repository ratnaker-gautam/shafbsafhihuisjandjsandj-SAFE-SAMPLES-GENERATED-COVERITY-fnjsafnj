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
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) new_capacity *= 2;
        if (new_capacity > MAX_OUTPUT_SIZE) exit(EXIT_FAILURE);
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    unsigned char *dst = buf->data + buf->size;
    const unsigned char *src_end = src + len;
    while (src < src_end) *dst++ = *src++;
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (!input || !output || input_len == 0 || output_capacity == 0) return 0;
    
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

size_t decompress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (!input || !output || input_len == 0 || output_capacity == 0 || input_len % 2 != 0) return 0;
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (in_ptr < in_end) {
        unsigned char value = *in_ptr++;
        unsigned char count = *in_ptr++;
        
        if (out_ptr + count > out_end) return 0;
        
        unsigned char *target_end = out_ptr + count;
        while (out_ptr < target_end) *out_ptr++ = value;
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    buffer_init(&compressed_buf, MAX_OUTPUT_SIZE);
    buffer_init(&decompressed_buf, MAX_INPUT_SIZE);
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    unsigned char temp_buf[128];
    
    while (bytes_read < MAX_INPUT_SIZE) {
        size_t chunk = fread(temp_buf, 1, sizeof(temp_buf), stdin);
        if (chunk == 0) break;
        
        if (bytes_read + chunk > MAX_INPUT_SIZE) {
            chunk = MAX_INPUT_SIZE - bytes_read;
        }
        
        buffer_append(&input_buf, temp_buf, chunk);
        bytes_read += chunk;
        
        if (chunk < sizeof(temp_buf)) break;
    }
    
    if (input_buf.size == 0) {
        printf("No input received.\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    unsigned char *compressed_data = malloc(MAX_OUTPUT_SIZE);
    if (!compressed_data) {
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed_data, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed.\n");
        free(compressed_data);
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&