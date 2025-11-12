//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t *data;
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

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
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
        uint8_t *new_data = realloc(buf->data, new_capacity);
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

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_len == 0 || output_capacity == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end && output_len + 2 <= output_capacity) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (output_len + 2 <= output_capacity) {
            output[output_len] = (uint8_t)count;
            output[output_len + 1] = value;
            output_len += 2;
        } else {
            break;
        }
        
        current += count;
    }
    
    return output_len;
}

int main(void) {
    Buffer input_buf;
    buffer_init(&input_buf, 256);
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        if (input_buf.size >= input_buf.capacity) {
            if (input_buf.capacity >= MAX_INPUT_SIZE) {
                break;
            }
            size_t new_capacity = input_buf.capacity * 2;
            if (new_capacity > MAX_INPUT_SIZE) {
                new_capacity = MAX_INPUT_SIZE;
            }
            uint8_t *new_data = realloc(input_buf.data, new_capacity);
            if (new_data == NULL) {
                buffer_free(&input_buf);
                return EXIT_FAILURE;
            }
            input_buf.data = new_data;
            input_buf.capacity = new_capacity;
        }
        input_buf.data[input_buf.size++] = (uint8_t)c;
        total_read++;
    }
    
    if (input_buf.size == 0) {
        printf("No input data provided.\n");
        buffer_free(&input_buf);
        return EXIT_FAILURE;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, compressed, MAX_OUTPUT_SIZE);
    
    printf("Original size: %zu bytes\n", input_buf.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (input_buf.size > 0) ? (100.0 * (double)compressed_size / (double)input_buf.size) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02X ", compressed[i]);
    }
    if (compressed_size > 32) {
        printf("...");
    }
    printf("\n");
    
    buffer_free(&input_buf);
    return EXIT_SUCCESS;
}