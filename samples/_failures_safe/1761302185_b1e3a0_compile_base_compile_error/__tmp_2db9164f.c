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

void buffer_append(Buffer *buf, const unsigned char *src, size_t len) {
    if (len == 0 || src == NULL) return;
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > MAX_OUTPUT_SIZE / 2) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    if (buf->size + len <= buf->capacity) {
        memcpy(buf->data + buf->size, src, len);
        buf->size += len;
    }
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

size_t simple_compress(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (in_ptr < in_end && out_ptr < out_end - 2) {
        unsigned char current = *in_ptr;
        size_t count = 1;
        
        const unsigned char *next = in_ptr + 1;
        while (next < in_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (count > 3 || current == 0xFF) {
            *out_ptr++ = 0xFF;
            if (out_ptr >= out_end) break;
            *out_ptr++ = current;
            if (out_ptr >= out_end) break;
            *out_ptr++ = (unsigned char)count;
            in_ptr += count;
        } else {
            for (size_t i = 0; i < count && out_ptr < out_end; i++) {
                *out_ptr++ = current;
            }
            in_ptr += count;
        }
    }
    
    return out_ptr - output;
}

size_t simple_decompress(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *in_ptr = input;
    const unsigned char *in_end = input + input_len;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (in_ptr < in_end && out_ptr < out_end) {
        if (*in_ptr == 0xFF && in_ptr + 2 < in_end) {
            unsigned char value = *(in_ptr + 1);
            unsigned char count = *(in_ptr + 2);
            
            if (out_ptr + count > out_end) {
                break;
            }
            
            for (unsigned char i = 0; i < count; i++) {
                *out_ptr++ = value;
            }
            in_ptr += 3;
        } else {
            *out_ptr++ = *in_ptr++;
        }
    }
    
    return out_ptr - output;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    buffer_init(&input_buf, 256);
    buffer_init(&compressed_buf, 512);
    buffer_init(&decompressed_buf, 256);
    
    unsigned char temp_input[MAX_INPUT_SIZE];
    size_t bytes_read = 0;
    
    while (bytes_read < MAX_INPUT_SIZE) {
        int ch = getchar();
        if (ch == EOF || ch == '\n') break;
        if (bytes_read < MAX_INPUT_SIZE) {
            temp_input[bytes_read++] = (unsigned char)ch;
        }
    }
    
    if (bytes_read > 0) {
        buffer_append(&input_buf, temp_input, bytes_read);
        
        unsigned char temp_output[MAX_OUTPUT_SIZE];
        size_t compressed_size = simple_compress(input_buf.data