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
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
            output.data[out_idx++] = 0xFF;
            output.data[out_idx++] = current;
            output.data[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            if (out_idx + count >= MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output.data[out_idx++] = input[i + j];
            }
            i += count;
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
            
            if (out_idx + count > MAX_OUTPUT_SIZE) break;
            
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
    uint8_t test_data[] = "AAAABBBCCCDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ";
    size_t test_size = strlen((char*)test_data);
    
    if (test_size > MAX_INPUT_SIZE) {
        fprintf(stderr, "Test data too large\n");
        return 1;
    }
    
    memcpy(input_data, test_data, test_size);
    
    printf("Original data size: %zu\n", test_size);
    printf("Original data: %s\n", input_data);
    
    Buffer compressed = compress_data(input_data, test_size);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (float)compressed.size / test_size * 100);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed size: %zu\n", decompressed.size);
    
    if (decompressed.size == test_size && memcmp(decompressed.data, input_data, test_size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}