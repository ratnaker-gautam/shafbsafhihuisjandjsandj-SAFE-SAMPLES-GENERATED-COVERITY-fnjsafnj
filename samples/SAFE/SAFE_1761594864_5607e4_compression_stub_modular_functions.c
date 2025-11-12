//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
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
    
    return 1;
}

int decompress_rle(const CompressedData* compressed, uint8_t* output, size_t output_size) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0) return 0;
    if (compressed->size % 2 != 0) return 0;
    
    size_t decompressed_size = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (decompressed_size + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_size++] = value;
        }
    }
    
    return decompressed_size;
}

void print_hex(const uint8_t* data, size_t size) {
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
    
    printf("Enter input data (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \n");
    size_t input_size = 0;
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) {
            printf("Invalid hex byte: %s\n", token);
            return 1;
        }
        input_data[input_size++] = (uint8_t)byte;
        token = strtok(NULL, " \n");
    }
    
    if (input_size == 0) {
        printf("No valid input data provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    print_hex(input_data, input_size);
    
    if (!compress_rle(input_data, input_size, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    size_t decompressed_size = decompress_rle(&compressed, output_data, sizeof(output_data));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    print_hex(output_data, decompressed_size);
    
    if (decompressed_size != input_size || memcmp(input_data, output_data, input_size) != 0) {
        printf("Data mismatch after compression/decompression\n");
        return 1;
    }
    
    printf("Compression/Decompression successful\n");
    printf("Compression ratio: %.2f%%\n", (float)compressed.size / input_size * 100.0f);
    
    return 0;
}