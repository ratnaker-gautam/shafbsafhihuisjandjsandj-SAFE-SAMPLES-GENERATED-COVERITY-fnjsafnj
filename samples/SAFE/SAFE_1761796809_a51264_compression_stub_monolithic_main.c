//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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
} CompressedData;

int compress_rle(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    if (input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output_index + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output_index++] = (uint8_t)count;
        output->data[output_index++] = current;
        
        i += count;
    }
    
    output->size = output_index;
    return 0;
}

int decompress_rle(const CompressedData* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
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
    CompressedData compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter input data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid data value\n");
            return 1;
        }
        if (value < 0 || value > 255) {
            printf("Data value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < input_size; i++) {
        printf("%d ", input_data[i]);
    }
    printf("\n");
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / (float)input_size * 100.0f);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    if (decompress_rle(&compressed, decompressed_data, &decompressed_size) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (decompressed_size != input_size) {
        printf("Decompression size mismatch\n");
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < input_size; i++) {
        if (input_data[i] != decompressed_data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression successful - data matches original\n");
    } else {
        printf("Decompression failed - data mismatch\n");
        return 1;
    }
    
    return 0;
}