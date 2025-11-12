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

int main() {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter data to compress (up to %d bytes as hex values, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_size = 0;
    char line[8192];
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02x", &value) != 1 || value > 255) {
            fprintf(stderr, "Invalid hex value: %s\n", token);
            return 1;
        }
        original[input_size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02x ", original[i]);
    }
    printf("\n");
    
    if (compress_rle(original, input_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100);
    
    if (decompress_rle(&compressed, decompressed, sizeof(decompressed)) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    int match = memcmp(original, decompressed, input_size) == 0;
    printf("Decompression verified: %s\n", match ? "SUCCESS" : "FAILED");
    
    if (!match) {
        printf("Decompressed data: ");
        for (size_t i = 0; i < input_size; i++) {
            printf("%02x ", decompressed[i]);
        }
        printf("\n");
    }
    
    return 0;
}