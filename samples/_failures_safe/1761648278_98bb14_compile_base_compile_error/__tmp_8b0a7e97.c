//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
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

int buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) new_capacity = buf->size + len;
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    
    while (in_ptr < in_end && out_ptr + 2 <= out_end) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        
        const unsigned char *next = in_ptr + 1;
        while (next < in_end && *next == current && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        if (count >= 3) {
            *out_ptr++ = current;
            *out_ptr++ = (unsigned char)count;
            in_ptr += count;
        } else {
            *out_ptr++ = current;
            in_ptr++;
        }
    }
    
    return out_ptr - output;
}

size_t decompress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) return 0;
    
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    
    while (in_ptr < in_end && out_ptr < out_end) {
        unsigned char current = *in_ptr++;
        
        if (in_ptr < in_end && *in_ptr >= 3) {
            unsigned char count = *in_ptr++;
            size_t bytes_to_write = (size_t)count;
            
            if (out_ptr + bytes_to_write > out_end) break;
            
            unsigned char *fill_ptr = out_ptr;
            while (fill_ptr < out_ptr + bytes_to_write) {
                *fill_ptr++ = current;
            }
            out_ptr += bytes_to_write;
        } else {
            *out_ptr++ = current;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE) ||
        !buffer_init(&compressed_buf, MAX_OUTPUT_SIZE) ||
        !buffer_init(&decompressed_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    unsigned char temp_buf[MAX_INPUT_SIZE];
    if (fgets((char*)temp_buf, sizeof(temp_buf), stdin) == NULL) {
        fprintf(stderr, "Input reading failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    size_t input_len = strlen((char*)temp_buf);
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
    
    if (!buffer_append(&input_buf, temp_buf, input_len)) {
        fprintf(stderr, "Input buffer append failed\n");
        buffer_free