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

void buffer_append(ByteBuffer *buf, const unsigned char *src, size_t count) {
    if (count == 0) return;
    if (buf->size > SIZE_MAX - count) abort();
    size_t new_size = buf->size + count;
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / 2) abort();
            new_capacity *= 2;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, count);
    buf->size = new_size;
}

void buffer_free(ByteBuffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || output_capacity == 0) return 0;
    if (input_size == 0) return 0;
    
    size_t output_size = 0;
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        const unsigned char *next = current + 1;
        
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        if (output_size + 2 > output_capacity) return 0;
        
        *(output + output_size) = (unsigned char)count;
        *(output + output_size + 1) = value;
        output_size += 2;
        
        current = next;
    }
    
    return output_size;
}

int main(void) {
    ByteBuffer input_buf;
    buffer_init(&input_buf, BUFFER_SIZE);
    
    printf("Enter data to compress (max %d bytes):\n", BUFFER_SIZE);
    
    unsigned char temp[BUFFER_SIZE];
    size_t total_read = 0;
    
    while (total_read < BUFFER_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        if (total_read < BUFFER_SIZE) {
            temp[total_read] = (unsigned char)c;
            total_read++;
        }
    }
    
    if (total_read > 0) {
        buffer_append(&input_buf, temp, total_read);
    }
    
    if (input_buf.size == 0) {
        printf("No input data provided.\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    size_t max_output_size = input_buf.size * 2;
    if (max_output_size < input_buf.size) {
        printf("Input too large to process.\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    unsigned char *output = malloc(max_output_size);
    if (output == NULL) {
        printf("Memory allocation failed.\n");
        buffer_free(&input_buf);
        return 1;
    }
    
    size_t compressed_size = compress_rle(input_buf.data, input_buf.size, output, max_output_size);
    
    if (compressed_size == 0) {
        printf("Compression failed or produced no output.\n");
    } else {
        printf("Original size: %zu bytes\n", input_buf.size);
        printf("Compressed size: %zu bytes\n", compressed_size);
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_buf.size) * 100.0);
        
        printf("Compressed data (hex): ");
        for (size_t i = 0; i < compressed_size; i++) {
            printf("%02X ", *(output + i));
        }
        printf("\n");
    }
    
    free(output);
    buffer_free(&input_buf);
    return 0;
}