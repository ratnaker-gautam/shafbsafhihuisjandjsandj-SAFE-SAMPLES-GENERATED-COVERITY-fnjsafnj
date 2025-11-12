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
        
        if (count >= 3) {
            if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
            output.data[out_idx++] = 0xFF;
            output.data[out_idx++] = current;
            output.data[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output.data[out_idx++] = current;
            i++;
        }
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
            
            if (out_idx + count > MAX_OUTPUT_SIZE) {
                break;
            }
            
            for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output.data[out_idx++] = value;
            }
            i += 3;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output.data[out_idx++] = input[i];
            i++;
        }
    }
    
    output.size = out_idx;
    return output;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int temp;
        if (scanf("%u", &temp) != 1) {
            printf("Invalid data\n");
            return 1;
        }
        if (temp > UINT8_MAX) {
            printf("Value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)temp;
    }
    
    printf("\nOriginal data: ");
    for (size_t i = 0; i < input_size; i++) {
        printf("%u ", input_data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data(input_data, input_size);
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%u ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%u ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input_size && 
        memcmp(input_data, decompressed.data, input_size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    double ratio = (double)compressed.size / input_size * 100;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}