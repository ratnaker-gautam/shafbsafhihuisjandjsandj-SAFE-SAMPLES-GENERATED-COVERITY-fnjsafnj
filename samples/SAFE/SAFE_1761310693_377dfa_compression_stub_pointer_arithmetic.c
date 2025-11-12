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

void compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return;
    
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
        
        unsigned char compressed[2] = {value, (unsigned char)count};
        buffer_append(output, compressed, 2);
        
        current = next;
    }
}

int read_input(unsigned char *buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return -1;
    
    size_t total_read = 0;
    while (total_read < max_size) {
        int c = getchar();
        if (c == EOF) break;
        if (c < 0 || c > UCHAR_MAX) continue;
        
        *(buffer + total_read) = (unsigned char)c;
        total_read++;
        
        if (c == '\n') break;
    }
    
    return (int)total_read;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    int input_size = read_input(input, MAX_INPUT_SIZE - 1);
    if (input_size <= 0) {
        printf("No input received\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    buffer_init(&compressed, 128);
    
    compress_rle(input, (size_t)input_size, &compressed);
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (double)compressed.size / (double)input_size * 100.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", *(compressed.data + i));
    }
    printf("\n");
    
    free(compressed.data);
    return EXIT_SUCCESS;
}