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
        if (new_capacity > MAX_OUTPUT_SIZE) new_capacity = MAX_OUTPUT_SIZE;
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
    if (input == NULL || output == NULL || input_size == 0) return -1;
    if (input_size > MAX_INPUT_SIZE) return -1;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && *(input + i + count) == current) {
            count++;
        }
        
        buffer_append(output, current);
        buffer_append(output, (uint8_t)count);
        
        i += count;
        
        if (output->size > MAX_OUTPUT_SIZE) return -1;
    }
    
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0) return -1;
    if (input_size % 2 != 0) return -1;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t value = *(input + i);
        uint8_t count = *(input + i + 1);
        
        for (uint8_t j = 0; j < count; j++) {
            buffer_append(output, value);
            if (output->size > MAX_OUTPUT_SIZE) return -1;
        }
        
        i += 2;
    }
    
    return 0;
}

void print_buffer(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", *(buf->data + i));
    }
    printf("\n");
}

int main() {
    uint8_t original_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t original_size = sizeof(original_data) / sizeof(original_data[0]);
    
    Buffer compressed;
    buffer_init(&compressed, 16);
    
    printf("Original data: ");
    for (size_t i = 0; i < original_size; i++) {
        printf("%02X ", *(original_data + i));
    }
    printf("\n");
    
    if (compress_rle(original_data, original_size, &compressed) == 0) {
        printf("Compressed: ");
        print_buffer(&compressed);
        
        Buffer decompressed;
        buffer_init(&decompressed, 16);
        
        if (decompress_rle(compressed.data, compressed.size, &decompressed) == 0) {
            printf("Decompressed: ");
            print_buffer(&decompressed);
            
            if (decompressed.size == original_size && 
                memcmp(decompressed.data, original_data, original_size) == 0) {
                printf("Verification: SUCCESS\n");
            } else {
                printf("Verification: FAILED\n");
            }
        } else {
            printf("Decompression failed\n");
        }
        
        buffer_free(&decompressed);
    } else {
        printf("Compression failed\n");
    }
    
    buffer_free(&compressed);
    return 0;
}