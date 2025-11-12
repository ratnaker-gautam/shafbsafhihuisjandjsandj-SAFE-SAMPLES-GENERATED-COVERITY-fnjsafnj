//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size] = value;
            output->size++;
        }
    }
    
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size] = (uint8_t)c;
        input_size++;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("Original data size: %zu bytes\n", input_size);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed: output buffer too small or invalid input.\n");
        return 1;
    }
    
    printf("Compressed data size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_size) * 100);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed: invalid compressed data.\n");
        return 1;
    }
    
    if (decompressed.size != input_size) {
        printf("Decompression error: size mismatch.\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed.data, input_size) != 0) {
        printf("Decompression error: data mismatch.\n");
        return 1;
    }
    
    printf("Decompression successful: data verified.\n");
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}