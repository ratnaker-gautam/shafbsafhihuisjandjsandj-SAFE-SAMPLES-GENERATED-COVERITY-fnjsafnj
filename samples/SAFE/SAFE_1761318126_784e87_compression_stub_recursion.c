//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size, size_t pos) {
    if (pos >= input_size) {
        return 0;
    }
    
    size_t count = 1;
    while (pos + count < input_size && count < 255 && input[pos] == input[pos + count]) {
        count++;
    }
    
    if (count > 1 || input[pos] == 0xFF) {
        if (*output_size < 2) {
            return -1;
        }
        output[0] = 0xFF;
        output[1] = (uint8_t)count;
        output[2] = input[pos];
        *output_size -= 3;
        return 3 + compress_rle(input, input_size, output + 3, output_size, pos + count);
    } else {
        if (*output_size < 1) {
            return -1;
        }
        output[0] = input[pos];
        *output_size -= 1;
        return 1 + compress_rle(input, input_size, output + 1, output_size, pos + 1);
    }
}

int decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size, size_t pos) {
    if (pos >= input_size) {
        return 0;
    }
    
    if (input[pos] == 0xFF && pos + 2 < input_size) {
        uint8_t count = input[pos + 1];
        uint8_t value = input[pos + 2];
        
        if (*output_size < count) {
            return -1;
        }
        
        for (int i = 0; i < count; i++) {
            output[i] = value;
        }
        
        *output_size -= count;
        return count + decompress_rle(input, input_size, output + count, output_size, pos + 3);
    } else {
        if (*output_size < 1) {
            return -1;
        }
        output[0] = input[pos];
        *output_size -= 1;
        return 1 + decompress_rle(input, input_size, output + 1, output_size, pos + 1);
    }
}

int main() {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input.data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input.size = strlen((char*)input.data);
    if (input.size > 0 && input.data[input.size - 1] == '\n') {
        input.data[input.size - 1] = '\0';
        input.size--;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    size_t comp_size = MAX_INPUT_SIZE;
    int comp_result = compress_rle(input.data, input.size, compressed.data, &comp_size, 0);
    
    if (comp_result < 0) {
        fprintf(stderr, "Compression failed: output buffer too small\n");
        return 1;
    }
    compressed.size = comp_result;
    
    size_t decomp_size = MAX_INPUT_SIZE;
    int decomp_result = decompress_rle(compressed.data, compressed.size, decompressed.data, &decomp_size, 0);
    
    if (decomp_result < 0) {
        fprintf(stderr, "Decompression failed: output buffer too small\n");
        return 1;
    }
    decompressed.size = decomp_result;
    
    if (input.size != decompressed.size || memcmp(input.data, decompressed.data, input.size) != 0) {
        fprintf(stderr, "Verification failed: decompressed data doesn't match original\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input.size) * 100);
    printf("Decompression successful: %s\n", decompressed.data);
    
    return 0;
}