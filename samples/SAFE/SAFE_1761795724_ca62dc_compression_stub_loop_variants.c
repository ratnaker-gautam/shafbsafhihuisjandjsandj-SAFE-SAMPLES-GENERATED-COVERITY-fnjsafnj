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
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
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

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    if (buf1 == NULL || buf2 == NULL) {
        return 0;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t original[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    size_t data_size = 0;
    for (int i = 0; i < 64; i++) {
        if (data_size >= MAX_INPUT_SIZE) {
            break;
        }
        original[data_size] = (uint8_t)(i % 16);
        data_size++;
    }
    
    for (int repeat = 0; repeat < 3; repeat++) {
        if (data_size >= MAX_INPUT_SIZE) {
            break;
        }
        original[data_size] = 0xFF;
        data_size++;
    }
    
    compress_run_length(original, data_size, &compressed);
    
    if (compressed.size > 0) {
        decompress_run_length(compressed.data, compressed.size, &decompressed);
    }
    
    if (decompressed.size == data_size && 
        compare_buffers(original, decompressed.data, data_size)) {
        printf("Compression test passed: %zu -> %zu -> %zu bytes\n", 
               data_size, compressed.size, decompressed.size);
    } else {
        printf("Compression test failed\n");
        return 1;
    }
    
    printf("Original: ");
    for (size_t i = 0; i < (data_size < 16 ? data_size : 16); i++) {
        printf("%02X ", original[i]);
    }
    printf("\n");
    
    printf("Compressed: ");
    for (size_t i = 0; i < (compressed.size < 16 ? compressed.size : 16); i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}