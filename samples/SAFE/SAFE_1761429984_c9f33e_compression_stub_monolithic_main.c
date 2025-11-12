//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const CompressedData* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    
    *output_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        if (i + 1 >= input->size) {
            return -1;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size] = value;
            (*output_size)++;
        }
    }
    
    return 0;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    CompressedData compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter input data (up to %d bytes as hex values, space separated): ", MAX_INPUT_SIZE);
    
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        unsigned int value;
        if (scanf("%2x", &value) != 1) {
            break;
        }
        input_data[i] = (uint8_t)value;
        input_size++;
        if (getchar() == '\n') {
            break;
        }
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / (double)input_size) * 100.0);
    
    if (decompress_rle(&compressed, output_data, &decompressed_size) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (decompressed_size != input_size) {
        printf("Decompression size mismatch.\n");
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < input_size; i++) {
        if (input_data[i] != output_data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression verified: data matches original.\n");
    } else {
        printf("Decompression error: data does not match original.\n");
        return 1;
    }
    
    return 0;
}