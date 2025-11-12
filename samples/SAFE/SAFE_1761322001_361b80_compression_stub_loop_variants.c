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

void compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        for (size_t j = i + 1; j < input_size && j < i + 255; j++) {
            if (input[j] == current) {
                count++;
            } else {
                break;
            }
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
}

void decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (in_idx + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        for (uint8_t i = 0; i < count; i++) {
            if (out_idx < MAX_INPUT_SIZE) {
                output[out_idx++] = value;
            } else {
                break;
            }
        }
    }
    
    *output_size = out_idx;
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t original_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && original_size < MAX_INPUT_SIZE - 1) {
        original[original_size++] = (uint8_t)c;
    }
    original[original_size] = '\0';
    
    if (original_size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    compress_rle(original, original_size, &compressed);
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.size / original_size * 100.0f);
    }
    
    decompress_rle(&compressed, decompressed, &decompressed_size);
    
    if (decompressed_size != original_size) {
        printf("Decompression error: size mismatch.\n");
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < original_size; i++) {
        if (original[i] != decompressed[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression successful: data verified.\n");
    } else {
        printf("Decompression failed: data corruption.\n");
        return 1;
    }
    
    return 0;
}