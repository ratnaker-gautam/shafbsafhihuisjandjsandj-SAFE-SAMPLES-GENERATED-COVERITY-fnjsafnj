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
        while (new_capacity < buf->size + len) {
            if (new_capacity > MAX_OUTPUT_SIZE) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (buf->size + len > new_capacity) {
            return;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    unsigned char *dest = buf->data + buf->size;
    const unsigned char *src_ptr = src;
    for (size_t i = 0; i < len; i++) {
        *dest++ = *src_ptr++;
    }
    buf->size += len;
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
    
    unsigned char *output_ptr = output;
    unsigned char *output_end = output + output_capacity;
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_len;
    
    while (current < end) {
        if (output_ptr >= output_end - 2) {
            break;
        }
        
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (count > 3 || value == 0xFF) {
            *output_ptr++ = 0xFF;
            if (output_ptr >= output_end) break;
            *output_ptr++ = value;
            if (output_ptr >= output_end) break;
            *output_ptr++ = (unsigned char)count;
        } else {
            for (size_t i = 0; i < count; i++) {
                if (output_ptr >= output_end) break;
                *output_ptr++ = value;
            }
        }
        
        current += count;
    }
    
    return output_ptr - output;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    
    printf("Enter data to compress (max %zu bytes): ", (size_t)MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        int ch = getchar();
        if (ch == EOF || ch == '\n') {
            break;
        }
        if (input_buf.size >= input_buf.capacity) {
            break;
        }
        input_buf.data[input_buf.size++] = (unsigned char)ch;
        total_read++;
    }
    
    if (input_buf.size == 0) {
        printf("No input data provided.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    
    unsigned char compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = simple_compress(input_buf.data, input_buf.size, compressed, MAX_OUTPUT_SIZE);
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (input_buf.size > 0) ? (100.0 * (double)compressed_size / (double)input_buf.size) : 0.0);
    
    printf("Compressed data (hex): ");
    unsigned char *comp_ptr = compressed;
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02X ", *comp_ptr++);
    }
    if (compressed_size > 32) {
        printf("...");
    }
    printf("\n");
    
    buffer_free(&input_buf);
    return EXIT_SUCCESS;
}