//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

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
        while (j < input_size && input[j] == current && count < UINT8_MAX) {
            count++;
            j++;
        }
        
        output->data[output->size] = current;
        output->data[output->size + 1] = (uint8_t)count;
        output->size += 2;
        
        i = j;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output->data[output->size] = value;
            output->size++;
            k++;
        } while (k < count);
    }
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[input_size - 1] = '\0';
        input_size--;
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    compress_run_length(input_data, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (input_size > 0) ? (100.0 * compressed.size / input_size) : 0.0);
    
    if (compressed.size > 0) {
        printf("Compressed data: ");
        for (size_t i = 0; i < compressed.size; i++) {
            printf("%02X ", compressed.data[i]);
        }
        printf("\n");
    }
    
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    if (decompressed.size > 0) {
        printf("Decompressed text: ");
        for (size_t i = 0; i < decompressed.size; i++) {
            printf("%c", decompressed.data[i]);
        }
        printf("\n");
        
        if (decompressed.size == input_size && 
            memcmp(input_data, decompressed.data, input_size) == 0) {
            printf("Verification: SUCCESS\n");
        } else {
            printf("Verification: FAILED\n");
        }
    }
    
    return 0;
}