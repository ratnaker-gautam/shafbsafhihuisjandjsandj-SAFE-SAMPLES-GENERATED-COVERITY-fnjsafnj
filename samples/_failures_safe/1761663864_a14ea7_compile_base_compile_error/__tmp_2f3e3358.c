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
    
    memcpy(buf->data + buf->size, src, len);
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

size_t compress_rle(const unsigned char *input, size_t input_size, unsigned char *output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0 || output_capacity == 0) {
        return 0;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    unsigned char *out_ptr = output;
    unsigned char *out_end = output + output_capacity;
    
    while (current < end && out_ptr < out_end - 2) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        *out_ptr++ = value;
        *out_ptr++ = (unsigned char)count;
        
        current += count;
    }
    
    return out_ptr - output;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    unsigned char output[MAX_OUTPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int c;
    while (bytes_read < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input[bytes_read++] = (unsigned char)c;
    }
    
    if (bytes_read == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    if (bytes_read == MAX_INPUT_SIZE) {
        int extra;
        while ((extra = getchar()) != EOF && extra != '\n') {
            bytes_read++;
        }
        if (bytes_read > MAX_INPUT_SIZE) {
            printf("Input too large. Maximum size is %d bytes.\n", MAX_INPUT_SIZE);
            return EXIT_FAILURE;
        }
    }
    
    size_t compressed_size = compress_rle(input, bytes_read, output, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        printf("Compression failed or produced no output.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", bytes_read);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / bytes_read) * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02x ", output[i]);
    }
    if (compressed_size > 32) {
        printf("...");
    }
    printf("\n");
    
    Buffer decompressed;
    buffer_init(&decompressed, bytes_read + 1);
    
    const unsigned char *comp_ptr = output;
    const unsigned char *comp_end = output + compressed_size;
    
    while (comp_ptr < comp_end - 1) {
        unsigned char value = *comp_ptr++;
        unsigned char count = *comp_ptr++;
        
        for (unsigned char i = 0; i < count; i++) {
            if (decompressed.size < decompressed.capacity) {
                *(decompressed.data + decompressed.size) = value;
                decompressed.size++;
            }
        }
    }
    
    if (decomp