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
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) exit(EXIT_FAILURE);
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = byte;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return -1;
    if (input_size > MAX_INPUT_SIZE) return -1;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && *(input + i + count) == current && count < UCHAR_MAX) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, current);
            buffer_append(output, (unsigned char)count);
        } else {
            for (size_t j = 0; j < count; j++) {
                buffer_append(output, current);
            }
        }
        
        i += count;
        
        if (output->size > MAX_OUTPUT_SIZE) return -1;
    }
    
    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return -1;
    
    size_t i = 0;
    while (i < input_size) {
        unsigned char current = *(input + i);
        
        if (current == 0xFF && i + 2 < input_size) {
            unsigned char value = *(input + i + 1);
            unsigned char count = *(input + i + 2);
            
            if (count == 0) return -1;
            
            for (unsigned char j = 0; j < count; j++) {
                buffer_append(output, value);
                if (output->size > MAX_OUTPUT_SIZE) return -1;
            }
            
            i += 3;
        } else {
            buffer_append(output, current);
            i++;
            if (output->size > MAX_OUTPUT_SIZE) return -1;
        }
    }
    
    return 0;
}

void print_hex(const unsigned char *data, size_t size) {
    if (!data || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", *(data + i));
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter input data (hex bytes, space separated, max %d bytes): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (!fgets(line, sizeof(line), stdin)) {
        printf("Input error\n");
        return EXIT_FAILURE;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1 || byte > 0xFF) {
            printf("Invalid hex byte: %s\n", token);
            return EXIT_FAILURE;
        }
        *(input_data + input_size) = (unsigned char)byte;
        input_size++;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        printf("No input data provided\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    Buffer compressed;
    buffer_init(&compressed, 64);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);