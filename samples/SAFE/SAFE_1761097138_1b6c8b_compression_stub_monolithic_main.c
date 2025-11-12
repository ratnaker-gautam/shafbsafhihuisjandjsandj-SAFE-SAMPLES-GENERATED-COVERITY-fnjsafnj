//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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
    for (size_t i = 0; i < input_size; ) {
        if (out_idx >= MAX_OUTPUT_SIZE - 2) {
            output.size = 0;
            return output;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output.data[out_idx++] = 0xFF;
            output.data[out_idx++] = current;
            output.data[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (out_idx >= MAX_OUTPUT_SIZE) {
                    output.size = 0;
                    return output;
                }
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
    if (input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; ) {
        if (out_idx >= MAX_INPUT_SIZE) {
            output.size = 0;
            return output;
        }
        
        if (i < input_size - 2 && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count == 0 || out_idx + count > MAX_INPUT_SIZE) {
                output.size = 0;
                return output;
            }
            
            for (uint8_t j = 0; j < count; j++) {
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
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (float)(data_size - compressed.size) / data_size * 100.0f);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0 || decompressed.size != data_size) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", decompressed.data[i]);
    }
    printf("\n");
    
    if (memcmp(original_data, decompressed.data, data_size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}