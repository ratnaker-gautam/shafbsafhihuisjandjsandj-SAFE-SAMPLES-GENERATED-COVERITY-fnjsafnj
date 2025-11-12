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
    if (input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_OUTPUT_SIZE; ) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count == 0 || out_idx + count > MAX_OUTPUT_SIZE) {
                output.size = 0;
                return output;
            }
            
            for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output.data[out_idx++] = value;
            }
            i += 3;
        } else {
            output.data[out_idx++] = input[i++];
        }
    }
    
    output.size = out_idx;
    return output;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t test_pattern[] = {1, 1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 4, 5};
    size_t test_size = sizeof(test_pattern);
    
    printf("Original data (%zu bytes): ", test_size);
    for (size_t i = 0; i < test_size; i++) {
        printf("%d ", test_pattern[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data(test_pattern, test_size);
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size != test_size) {
        printf("Size mismatch: expected %zu, got %zu\n", test_size, decompressed.size);
        return 1;
    }
    
    if (memcmp(test_pattern, decompressed.data, test_size) != 0) {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / test_size) * 100);
    
    return 0;
}