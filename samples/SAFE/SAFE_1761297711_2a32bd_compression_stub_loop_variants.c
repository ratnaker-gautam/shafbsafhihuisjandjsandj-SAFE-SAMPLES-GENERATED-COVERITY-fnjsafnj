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

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
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
        
        i = j;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    for (i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t j = 0;
        while (j < count) {
            output->data[output->size] = value;
            output->size++;
            j++;
        }
    }
}

int main() {
    uint8_t original[MAX_INPUT_SIZE];
    Buffer compressed;
    Buffer decompressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)original, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
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
    
    compress_run_length(original, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (input_size == decompressed.size && 
        memcmp(original, decompressed.data, input_size) == 0) {
        printf("Decompression successful: %s\n", decompressed.data);
    } else {
        printf("Decompression failed\n");
        return 1;
    }
    
    return 0;
}