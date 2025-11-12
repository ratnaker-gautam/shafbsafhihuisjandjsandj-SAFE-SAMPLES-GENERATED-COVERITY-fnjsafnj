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
} Buffer;

Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_OUTPUT_SIZE - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && 
               input[i + count] == current && out_idx < MAX_OUTPUT_SIZE - 2) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output.data[out_idx++] = 0xFF;
            output.data[out_idx++] = current;
            output.data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output.data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    output.size = out_idx;
    return output;
}

Buffer decompress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_OUTPUT_SIZE; ) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count == 0) {
                break;
            }
            
            for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output.data[out_idx++] = value;
            }
            i += 3;
        } else {
            output.data[out_idx++] = input[i];
            i++;
        }
    }
    
    output.size = out_idx;
    return output;
}

int main() {
    uint8_t original_data[MAX_INPUT_SIZE];
    size_t data_size = 0;
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (data_size == 0 || data_size > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        if (value < 0 || value > 255) {
            printf("Value out of range\n");
            return 1;
        }
        original_data[i] = (uint8_t)value;
    }
    
    printf("\nOriginal data: ");
    for (size_t i = 0; i < data_size; i++) {
        printf("%d ", original_data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data(original_data, data_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    printf("Decompressed size: %zu\n", decompressed.size);
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == data_size && 
        memcmp(original_data, decompressed.data, data_size) == 0) {
        printf("Compression successful\n");
    } else {
        printf("Compression failed\n");
    }
    
    return 0;
}