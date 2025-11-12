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
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 < output_max) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < UINT8_MAX && 
               input[in_idx + count] == current && out_idx + 2 < output_max) {
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
    if (input == NULL || output == NULL || output_max == 0) return 0;
    
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
            if (out_idx >= output_max) return out_idx;
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

int compress_data(const char* input, size_t input_len, Buffer* compressed) {
    if (!validate_input(input, input_len) || compressed == NULL) return 0;
    
    size_t result_size = simple_compress((const uint8_t*)input, input_len, 
                                        compressed->data, MAX_OUTPUT_SIZE);
    
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) return 0;
    
    compressed->size = result_size;
    return 1;
}

int decompress_data(const Buffer* compressed, char* output, size_t output_max) {
    if (compressed == NULL || output == NULL || output_max == 0) return 0;
    if (compressed->size == 0 || compressed->size > MAX_OUTPUT_SIZE) return 0;
    
    size_t result_size = simple_decompress(compressed->data, compressed->size,
                                          (uint8_t*)output, output_max);
    
    if (result_size == 0 || result_size > output_max) return 0;
    
    output[result_size] = '\0';
    return 1;
}

void demonstrate_compression(const char* test_string) {
    if (test_string == NULL) return;
    
    size_t len = strlen(test_string);
    if (len == 0 || len > MAX_INPUT_SIZE) return;
    
    Buffer compressed = {0};
    char decompressed[MAX_INPUT_SIZE] = {0};
    
    printf("Original: %s\n", test_string);
    printf("Original size: %zu bytes\n", len);
    
    if (compress_data(test_string, len, &compressed)) {
        printf("Compressed size: %zu bytes\n", compressed.size);
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.size / len * 100.0f);
        
        if (decompress_data(&compressed, decompressed, MAX_INPUT_SIZE)) {
            printf("Decompressed: %s\n", decompressed);
            
            if (strcmp(test_string, decompressed) == 0) {
                printf("Verification: SUCCESS\n");
            } else {
                printf("Verification: FAILED\n");
            }
        } else {
            printf("Decompression failed\n");
        }
    } else {
        printf("Compression failed\n");
    }
    printf("\n");
}

int main() {
    const char* test1 = "AAAAABBBBBCCCCCDDDDD";
    const char* test2 = "Hello World!";
    const char* test3 = "abcabcabcabc";
    const char* test4 = "A";
    
    demonstrate_compression(test1);
    demonstrate_compression(test2);
    demonstrate_compression(test3);
    demonstrate_