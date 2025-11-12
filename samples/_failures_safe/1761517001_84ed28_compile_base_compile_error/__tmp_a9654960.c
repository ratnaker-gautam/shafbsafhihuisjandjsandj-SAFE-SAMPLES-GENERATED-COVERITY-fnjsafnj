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
    if (initial_capacity > SIZE_MAX / 2) return 0;
    
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
    
    if (buf->size > SIZE_MAX - len) return 0;
    size_t new_size = buf->size + len;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_ptr = src;
    for (size_t i = 0; i < len; i++) {
        *dest++ = *src_ptr++;
    }
    buf->size = new_size;
    return 1;
}

int compress_rle(const unsigned char *input, size_t input_len, 
                 unsigned char *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return 0;
    if (input_len == 0 || *output_len < input_len * 2) return 0;
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + *output_len;
    
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
    
    *output_len = out_ptr - output;
    return 1;
}

int decompress_rle(const unsigned char *input, size_t input_len,
                   unsigned char *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return 0;
    if (input_len % 2 != 0) return 0;
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + *output_len;
    
    while (in_ptr < in_end) {
        if (in_ptr + 2 > in_end) return 0;
        
        unsigned char value = *in_ptr++;
        unsigned char count = *in_ptr++;
        
        if (out_ptr + count > out_end) return 0;
        
        for (unsigned char i = 0; i < count; i++) {
            *out_ptr++ = value;
        }
    }
    
    *output_len = out_ptr - output;
    return 1;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    if (!buffer_init(&input_buf, MAX_INPUT_SIZE) ||
        !buffer_init(&compressed_buf, MAX_OUTPUT_SIZE) ||
        !buffer_init(&decompressed_buf, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter text to compress (max %zu characters): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Input reading failed\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        buffer_free(&input_buf);
        buffer_free