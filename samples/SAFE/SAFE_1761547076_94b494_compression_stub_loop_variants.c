//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    output->size = out_idx;
}

void decompress_rle(const Buffer* input, uint8_t* output, size_t max_output_size, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        *output_size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx + 1 < input->size) {
        uint8_t value = input->data[in_idx++];
        uint8_t count = input->data[in_idx++];
        
        for (uint8_t k = 0; k < count; k++) {
            if (out_idx >= max_output_size) {
                *output_size = out_idx;
                return;
            }
            output[out_idx++] = value;
        }
    }
    
    *output_size = out_idx;
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size;
    
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
        printf("No input provided\n");
        return 1;
    }
    
    compress_rle(original, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.size / input_size * 100.0f);
    }
    
    decompress_rle(&compressed, decompressed, MAX_INPUT_SIZE, &decompressed_size);
    
    if (decompressed_size == input_size && 
        memcmp(original, decompressed, input_size) == 0) {
        printf("Decompression successful - data verified\n");
    } else {
        printf("Decompression failed - data mismatch\n");
        return 1;
    }
    
    printf("First 10 bytes of compressed data: ");
    for (size_t i = 0; i < (compressed.size < 10 ? compressed.size : 10); i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}