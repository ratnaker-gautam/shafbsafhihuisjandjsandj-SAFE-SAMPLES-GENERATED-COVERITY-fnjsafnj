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
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const unsigned char *src, size_t count) {
    if (count == 0 || src == NULL) return;
    
    if (buf->size > SIZE_MAX - count) {
        exit(EXIT_FAILURE);
    }
    
    size_t new_size = buf->size + count;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) {
            exit(EXIT_FAILURE);
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        if (new_size > new_capacity) {
            exit(EXIT_FAILURE);
        }
        
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_end = src + count;
    while (src < src_end) {
        *dest = *src;
        dest++;
        src++;
    }
    buf->size = new_size;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t simple_compress(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *input_end = input + input_size;
    unsigned char *output_ptr = output;
    unsigned char *output_end = output + output_capacity;
    
    while (input < input_end) {
        if (output_ptr >= output_end - 2) {
            return 0;
        }
        
        unsigned char current = *input;
        size_t count = 1;
        
        const unsigned char *next = input + 1;
        while (next < input_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (count > 1) {
            *output_ptr = current;
            output_ptr++;
            *output_ptr = (unsigned char)count;
            output_ptr++;
        } else {
            *output_ptr = current;
            output_ptr++;
        }
        
        input += count;
    }
    
    return output_ptr - output;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t bytes_read = 0;
    
    while (bytes_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF) break;
        if (bytes_read == MAX_INPUT_SIZE) break;
        input[bytes_read] = (unsigned char)c;
        bytes_read++;
    }
    
    if (bytes_read == 0) {
        return EXIT_SUCCESS;
    }
    
    Buffer compressed;
    buffer_init(&compressed, bytes_read * 2);
    
    size_t compressed_size = simple_compress(input, bytes_read, compressed.data, compressed.capacity);
    if (compressed_size == 0) {
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    compressed.size = compressed_size;
    
    const unsigned char *output_ptr = compressed.data;
    const unsigned char *output_end = compressed.data + compressed.size;
    while (output_ptr < output_end) {
        if (putchar(*output_ptr) == EOF) {
            buffer_free(&compressed);
            return EXIT_FAILURE;
        }
        output_ptr++;
    }
    
    buffer_free(&compressed);
    return EXIT_SUCCESS;
}