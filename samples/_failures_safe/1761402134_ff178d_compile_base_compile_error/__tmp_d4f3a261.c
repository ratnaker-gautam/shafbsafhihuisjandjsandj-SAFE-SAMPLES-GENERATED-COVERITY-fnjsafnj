//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 < output_max) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            for (size_t i = 0; i < count && out_idx < output_max; i++) {
                output[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_max) {
        if (in_idx + 2 < input_len && input[in_idx] == 0xFF) {
            uint8_t value = input[in_idx + 1];
            uint8_t count = input[in_idx + 2];
            
            if (out_idx + count > output_max) return out_idx;
            
            for (uint8_t i = 0; i < count; i++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

int test_compression(const uint8_t* original, size_t original_len) {
    if (original == NULL || original_len == 0) return 0;
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    size_t compressed_size = simple_compress(original, original_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) return 0;
    
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    if (decompressed_size != original_len) return 0;
    
    if (memcmp(original, decompressed, original_len) != 0) return 0;
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = simple_compress((uint8_t*)input_buffer, input_len, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (input_len > 0) {
        double ratio = (double)compressed_size / input_len * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, MAX_INPUT_SIZE);
    
    if (decompressed_size != input_len) {
        fprintf(stderr, "Decompression size mismatch\n");
        return 1;
    }
    
    decompressed[decompressed_size] = '\0';
    printf("Decompressed text: %s\n", decompressed);
    
    if (test_compression((uint8_t*)input_buffer, input_len)) {
        printf("Compression test passed\n");
    } else {
        printf("Compression test failed\n