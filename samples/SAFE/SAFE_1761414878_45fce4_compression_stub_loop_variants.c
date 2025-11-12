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
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        
        i += count;
    }
}

void decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return;
    }
    
    *output_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size] = value;
            (*output_size)++;
        }
    }
}

int main() {
    uint8_t original[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)original, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t original_size = strlen((char*)original);
    if (original_size > 0 && original[original_size - 1] == '\n') {
        original[--original_size] = '\0';
    }
    
    if (original_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    compress_rle(original, original_size, &compressed);
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.size / original_size * 100.0f);
    }
    
    size_t decompressed_size;
    decompress_rle(&compressed, decompressed, &decompressed_size);
    
    if (decompressed_size != original_size) {
        fprintf(stderr, "Decompression size mismatch\n");
        return 1;
    }
    
    if (memcmp(original, decompressed, original_size) != 0) {
        fprintf(stderr, "Decompression data mismatch\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    printf("Original: %s\n", original);
    printf("Decompressed: %s\n", decompressed);
    
    return 0;
}