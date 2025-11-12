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
        while (j < input_size && j - i < 255 && input[j] == current) {
            count++;
            j++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        
        i += count;
    }
}

void decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size == 0) {
        *output_size = 0;
        return;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    for (size_t idx = 0; idx < input->size; idx += 2) {
        if (idx + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[idx];
        uint8_t value = input->data[idx + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output[*output_size + k] = value;
            k++;
        } while (k < count);
        
        *output_size += count;
    }
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)original, sizeof(original), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)original);
    if (input_len > 0 && original[input_len - 1] == '\n') {
        original[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    compress_rle(original, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.size / input_len * 100.0f);
    }
    
    decompress_rle(&compressed, decompressed, &decompressed_size);
    
    if (decompressed_size != input_len) {
        fprintf(stderr, "Decompression size mismatch\n");
        return 1;
    }
    
    int match = 1;
    for (size_t i = 0; i < input_len; i++) {
        if (original[i] != decompressed[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("First 50 bytes of compressed data (hex): ");
    size_t display_len = compressed.size < 50 ? compressed.size : 50;
    for (size_t i = 0; i < display_len; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}