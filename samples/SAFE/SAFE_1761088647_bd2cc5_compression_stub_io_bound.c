//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

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

int decompress_rle(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input->size % 2 != 0) {
        return -1;
    }
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (output_index + count > output_size) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_index++] = value;
        }
    }
    
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("Original data size: %zu bytes\n", input_size);
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data size: %zu bytes\n", compressed.size);
    
    if (compressed.size >= input_size) {
        printf("No compression achieved.\n");
    } else {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.size / input_size) * 100.0);
    }
    
    if (decompress_rle(&compressed, decompressed, sizeof(decompressed)) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed, input_size) == 0) {
        printf("Decompression verified: data matches original.\n");
    } else {
        printf("Decompression error: data mismatch.\n");
        return 1;
    }
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}