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
    if (initial_capacity == 0) initial_capacity = 16;
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
        
        while (i + count < input_size && count < UCHAR_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, (unsigned char)count);
            buffer_append(output, current);
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
        
        if (current == 0xFF) {
            if (i + 2 >= input_size) return -1;
            
            unsigned char count = *(input + i + 1);
            unsigned char value = *(input + i + 2);
            
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

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int temp;
        if (scanf("%u", &temp) != 1) {
            printf("Invalid input byte\n");
            return EXIT_FAILURE;
        }
        if (temp > UCHAR_MAX) {
            printf("Invalid input byte\n");
            return EXIT_FAILURE;
        }
        *(input_data + i) = (unsigned char)temp;
    }
    
    Buffer compressed;
    buffer_init(&compressed, 16);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%u ", *(compressed.data + i));
    }
    printf("\n");
    
    Buffer decompressed;
    buffer_init(&decompressed, 16);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size