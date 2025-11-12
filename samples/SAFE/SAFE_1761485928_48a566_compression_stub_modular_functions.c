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

size_t compress_rle(const char* input, size_t input_len, Buffer* output) {
    if (output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (out_idx + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_len && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        output->data[out_idx++] = count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
    return out_idx;
}

size_t decompress_rle(const Buffer* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (in_idx + 1 >= input->size) return 0;
        
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int compare_data(const char* original, const char* decompressed, size_t len) {
    if (original == NULL || decompressed == NULL) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (original[i] != decompressed[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    char decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    
    size_t compressed_size = compress_rle(input, input_len, &compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / input_len) * 100.0);
    
    size_t decompressed_size = decompress_rle(&compressed, decompressed, sizeof(decompressed));
    if (decompressed_size != input_len) {
        fprintf(stderr, "Decompression size mismatch\n");
        return 1;
    }
    
    if (!compare_data(input, decompressed, input_len)) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression successful - data verified\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}