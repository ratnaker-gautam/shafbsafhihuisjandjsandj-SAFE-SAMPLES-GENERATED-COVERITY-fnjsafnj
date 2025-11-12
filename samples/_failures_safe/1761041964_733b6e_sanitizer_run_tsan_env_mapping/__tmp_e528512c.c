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
            output.data[out_idx++] = (uint8_t)count;
            output.data[out_idx++] = current;
        } else {
            for (size_t j = 0; j < count; j++) {
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
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        if (out_idx >= MAX_OUTPUT_SIZE) {
            output.size = 0;
            return output;
        }
        
        if (input[in_idx] == 0xFF && in_idx + 2 < input_size) {
            uint8_t count = input[in_idx + 1];
            uint8_t value = input[in_idx + 2];
            
            if (out_idx + count > MAX_OUTPUT_SIZE) {
                output.size = 0;
                return output;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output.data[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output.data[out_idx++] = input[in_idx++];
        }
    }
    
    output.size = out_idx;
    return output;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t test_data[] = {1, 1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 4, 5};
    size_t test_size = sizeof(test_data);
    
    if (test_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Test data too large\n");
        return 1;
    }
    
    memcpy(input_data, test_data, test_size);
    
    printf("Original data (%zu bytes): ", test_size);
    for (size_t i = 0; i < test_size; i++) {
        printf("%d ", input_data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data(input_data, test_size);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size != test_size) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed.data, test_size) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    printf("Compression ratio: %.2f%%\n", 
           (float)(test_size - compressed.size) / test_size * 100.0f);
    
    return 0;
}