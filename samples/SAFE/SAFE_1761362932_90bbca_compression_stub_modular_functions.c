//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (out_idx + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && 
               input->data[i + count] == current) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
    return 1;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input.size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input.size == 0 || input.size > MAX_INPUT_SIZE) {
        printf("Invalid size range\n");
        return 1;
    }
    
    printf("Enter %zu bytes as hex values (00-FF): ", input.size);
    for (size_t i = 0; i < input.size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) {
            printf("Invalid hex value\n");
            return 1;
        }
        input.data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input.data, input.size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    printf("Original data: ");
    print_buffer_hex(&input);
    
    if (!compress_rle(&input, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    print_buffer_hex(&compressed);
    printf("Compression ratio: %.2f\n", (float)input.size / compressed.size);
    
    if (!decompress_rle(&compressed, &decompressed)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data: ");
    print_buffer_hex(&decompressed);
    
    if (input.size != decompressed.size) {
        printf("Size mismatch error\n");
        return 1;
    }
    
    if (memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Data mismatch error\n");
        return 1;
    }
    
    printf("Compression/Decompression successful\n");
    return 0;
}