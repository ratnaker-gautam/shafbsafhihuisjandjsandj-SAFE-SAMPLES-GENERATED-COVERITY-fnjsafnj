//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_size) break;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size) {
        if (in_idx + 1 >= input_size) break;
        
        uint8_t count = input[in_idx++];
        uint8_t value = input[in_idx++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char temp_input[MAX_INPUT_SIZE + 1];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    temp_input[strcspn(temp_input, "\n")] = 0;
    
    size_t input_len = strlen(temp_input);
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    memcpy(input_data.data, temp_input, input_len);
    input_data.size = input_len;
    
    if (!validate_input(input_data.data, input_data.size)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_data.size);
    print_hex(input_data.data, input_data.size);
    
    compressed.size = compress_rle(input_data.data, input_data.size, compressed.data, MAX_OUTPUT_SIZE);
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    decompressed.size = decompress_rle(compressed.data, compressed.size, decompressed.data, MAX_INPUT_SIZE);
    if (decompressed.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == input_data.size && memcmp(decompressed.data, input_data.data, input_data.size) == 0) {
        printf("\nVerification: SUCCESS - Data matches original\n");
        double ratio = (double)compressed.size / input_data.size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("\nVerification: FAILED - Data does not match original\n");
        return 1;
    }
    
    return 0;
}