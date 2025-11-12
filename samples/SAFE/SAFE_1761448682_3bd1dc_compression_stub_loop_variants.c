//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void simple_compress(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            output->size = 0;
            return;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
}

void simple_decompress(const Buffer* input, uint8_t* output, size_t* output_size) {
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input->size) {
        if (in_idx + 1 >= input->size) {
            *output_size = 0;
            return;
        }
        
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > MAX_INPUT_SIZE) {
            *output_size = 0;
            return;
        }
        
        for (uint8_t k = 0; k < count; k++) {
            output[out_idx++] = value;
        }
    }
    
    *output_size = out_idx;
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)original, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)original);
    if (input_size > 0 && original[input_size - 1] == '\n') {
        original[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    simple_compress(original, input_size, &compressed);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100);
    
    size_t decompressed_size;
    simple_decompress(&compressed, decompressed, &decompressed_size);
    
    if (decompressed_size == 0 || decompressed_size != input_size) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    int match = 1;
    for (size_t idx = 0; idx < input_size; idx++) {
        if (original[idx] != decompressed[idx]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression verified: data matches original\n");
    } else {
        fprintf(stderr, "Decompression error: data mismatch\n");
        return 1;
    }
    
    printf("First 50 bytes of compressed data (hex): ");
    size_t display_size = compressed.size < 50 ? compressed.size : 50;
    for (size_t pos = 0; pos < display_size; pos++) {
        printf("%02X ", compressed.data[pos]);
    }
    printf("\n");
    
    return 0;
}