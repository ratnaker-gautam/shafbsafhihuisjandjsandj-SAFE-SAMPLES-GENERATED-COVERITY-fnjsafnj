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
            output->data[output->size++] = value;
        }
    }
    
    return 0;
}

int main() {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_data.size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02x", &byte) != 1 || byte > 0xFF) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        input_data.data[input_data.size++] = (uint8_t)byte;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_data.size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_data.size);
    for (size_t i = 0; i < input_data.size; i++) {
        printf("%02x ", input_data.data[i]);
    }
    printf("\n");
    
    if (compress_rle(input_data.data, input_data.size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%02x ", decompressed.data[i]);
    }
    printf("\n");
    
    if (input_data.size != decompressed.size || memcmp(input_data.data, decompressed.data, input_data.size) != 0) {
        fprintf(stderr, "Data mismatch after compression/decompression\n");
        return 1;
    }
    
    printf("Compression successful. Ratio: %.2f%%\n", 
           (100.0 * compressed.size) / input_data.size);
    
    return 0;
}