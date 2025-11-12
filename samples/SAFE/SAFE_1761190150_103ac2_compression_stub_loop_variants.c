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
    size_t decompressed_size;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)original, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)original);
    if (input_len > 0 && original[input_len - 1] == '\n') {
        original[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    compress_rle(original, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size >= input_len) {
        printf("No compression achieved\n");
    } else {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.size / input_len) * 100.0);
    }
    
    decompress_rle(&compressed, decompressed, &decompressed_size);
    
    if (decompressed_size != input_len) {
        fprintf(stderr, "Decompression size mismatch\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (original[i] != decompressed[i]) {
            fprintf(stderr, "Decompression data mismatch at position %zu\n", i);
            return 1;
        }
    }
    
    printf("Decompression verified successfully\n");
    
    return 0;
}