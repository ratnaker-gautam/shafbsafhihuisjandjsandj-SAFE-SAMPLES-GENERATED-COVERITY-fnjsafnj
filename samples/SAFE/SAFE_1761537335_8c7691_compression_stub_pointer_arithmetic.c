//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity <= buf->capacity) exit(EXIT_FAILURE);
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 1;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        buffer_append(output, value);
        buffer_append(output, (uint8_t)count);
        
        current = next;
    }
    
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0) return 1;
    if (input_size % 2 != 0) return 0;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_size;
    
    while (current < end) {
        uint8_t value = *current;
        current++;
        uint8_t count = *current;
        current++;
        
        for (uint8_t i = 0; i < count; i++) {
            buffer_append(output, value);
        }
    }
    
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size] = (uint8_t)c;
        input_size++;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    buffer_init(&compressed, input_size * 2);
    
    if (!compress_rle(input_data, input_size, &compressed)) {
        printf("Compression failed.\n");
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100.0f);
    
    Buffer decompressed;
    buffer_init(&decompressed, input_size);
    
    if (!decompress_rle(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    if (decompressed.size != input_size || 
        memcmp(decompressed.data, input_data, input_size) != 0) {
        printf("Decompression verification failed.\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully.\n");
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", *(compressed.data + i));
    }
    printf("\n");
    
    buffer_free(&compressed);
    buffer_free(&decompressed);
    
    return EXIT_SUCCESS;
}