//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_index + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[out_index++] = (uint8_t)count;
        output->data[out_index++] = current;
        i += count;
    }
    
    output->size = out_index;
    return 1;
}

int decompress_rle(const CompressedData* compressed, uint8_t* output, size_t* output_size) {
    if (compressed == NULL || output == NULL || output_size == NULL) return 0;
    if (compressed->size == 0 || compressed->size > MAX_OUTPUT_SIZE) return 0;
    if (compressed->size % 2 != 0) return 0;
    
    size_t out_index = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (out_index + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_index++] = value;
        }
    }
    
    *output_size = out_index;
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
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
    
    printf("Enter %zu bytes as hexadecimal values: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) {
            printf("Invalid input data\n");
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    printf("Original data: ");
    print_hex(input_data, input_size);
    
    if (!compress_rle(input_data, input_size, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    if (!decompress_rle(&compressed, output_data, &decompressed_size)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    print_hex(output_data, decompressed_size);
    
    if (decompressed_size != input_size) {
        printf("Size mismatch\n");
        return 1;
    }
    
    if (memcmp(input_data, output_data, input_size) != 0) {
        printf("Data mismatch\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}