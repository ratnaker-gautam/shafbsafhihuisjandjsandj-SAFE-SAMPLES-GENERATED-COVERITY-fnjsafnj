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

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size == 0) {
        return -1;
    }
    
    if (input->size % 2 != 0) {
        return -1;
    }
    
    size_t decompressed_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (decompressed_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_size++] = value;
        }
    }
    
    *output_size = decompressed_size;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter input data (up to %d bytes, end with EOF): ", MAX_INPUT_SIZE);
    
    int c;
    while (input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF) {
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    if (input_size == MAX_INPUT_SIZE && getchar() != EOF) {
        printf("Input too large.\n");
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
        printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_size) * 100.0);
    }
    
    if (decompress_rle(&compressed, decompressed_data, &decompressed_size) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (decompressed_size != input_size) {
        printf("Decompression size mismatch.\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Decompression data mismatch.\n");
        return 1;
    }
    
    printf("Decompression verified successfully.\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
        if (i < compressed.size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}