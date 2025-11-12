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
        while (new_capacity < buf->size + len) {
            if (new_capacity > MAX_OUTPUT_SIZE / 2) {
                new_capacity = MAX_OUTPUT_SIZE;
                break;
            }
            new_capacity *= 2;
        }
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
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
    const unsigned char *src_end = src + len;
    while (src < src_end) {
        *dest = *src;
        dest++;
        src++;
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

size_t compress_rle(const unsigned char *input, size_t input_len, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *input_end = input + input_len;
    unsigned char *output_ptr = output;
    unsigned char *output_end = output + output_capacity;
    
    while (input < input_end && output_ptr < output_end - 2) {
        unsigned char current = *input;
        size_t count = 1;
        
        const unsigned char *next = input + 1;
        while (next < input_end && *next == current && count < 255) {
            count++;
            next++;
        }
        
        if (count >= 3) {
            *output_ptr = current;
            output_ptr++;
            *output_ptr = (unsigned char)count;
            output_ptr++;
            input += count;
        } else {
            *output_ptr = current;
            output_ptr++;
            input++;
        }
    }
    
    return output_ptr - output;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, MAX_INPUT_SIZE);
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        if (input_buf.size >= input_buf.capacity) {
            break;
        }
        *(input_buf.data + input_buf.size) = (unsigned char)c;
        input_buf.size++;
        total_read++;
    }
    
    if (input_buf.size == 0) {
        printf("No input provided.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    
    unsigned char compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, MAX_OUTPUT_SIZE);
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (input_buf.size > 0) ? (100.0 * (double)compressed_size / (double)input_buf.size) : 0.0);
    
    printf("Compressed data (hex): ");
    unsigned char *comp_ptr = compressed;
    unsigned char *comp_end = compressed + compressed_size;
    while (comp_ptr < comp_end && comp_ptr < compressed + 32) {
        printf("%02x ", *comp_ptr);
        comp_ptr++;
    }
    if (compressed_size > 32) {
        printf("...");
    }
    printf("\n");
    
    buffer_free(&input_buf);
    return EXIT_SUCCESS;
}