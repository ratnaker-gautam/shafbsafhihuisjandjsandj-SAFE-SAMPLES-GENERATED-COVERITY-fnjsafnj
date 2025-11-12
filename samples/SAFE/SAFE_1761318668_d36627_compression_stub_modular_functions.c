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
    if (len == 0) return 0;
    if (len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_max - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output[out_idx++] = 0xFF;
            if (out_idx >= output_max) break;
            output[out_idx++] = current;
            if (out_idx >= output_max) break;
            output[out_idx++] = (uint8_t)count;
            if (out_idx >= output_max) break;
        } else {
            for (size_t j = 0; j < count && out_idx < output_max; j++) {
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_max) {
        if (input[i] == 0xFF && i + 2 < input_len) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count > output_max) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            
            i += 3;
        } else {
            output[out_idx++] = input[i++];
        }
    }
    
    return out_idx;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t len) {
    if (buf1 == NULL || buf2 == NULL) return 0;
    for (size_t i = 0; i < len; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

int process_compression(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (!validate_input(input_str, input_len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    memcpy(input_data, input_str, input_len);
    
    size_t compressed_size = simple_compress(input_data, input_len, compressed, sizeof(compressed));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (!compare_buffers(input_data, decompressed, input_len)) {
        printf("Verification failed - data corrupted\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_len) * 100);
    printf("Data verified successfully\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02X ", compressed[i]);
    }
    if (compressed_size > 32) printf("...");
    printf("\n");
    
    return 0;
}

int main(void) {
    return process_compression();
}