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
        
        for (; j < input_size && j < i + 255; j++) {
            if (input[j] != current) {
                break;
            }
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i = j;
    }
}

void decompress_run_length(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return;
    }
    
    *output_size = 0;
    size_t i = 0;
    
    do {
        if (i + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        while (k < count) {
            output[*output_size + k] = value;
            k++;
        }
        
        *output_size += count;
        i += 2;
    } while (i < input->size);
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_len < MAX_INPUT_SIZE - 1) {
        input_data[input_len++] = (uint8_t)c;
    }
    input_data[input_len] = '\0';
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    compress_run_length(input_data, input_len, &compressed);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / (double)input_len) * 100.0);
    
    size_t decompressed_size;
    decompress_run_length(&compressed, output_data, &decompressed_size);
    
    if (decompressed_size != input_len) {
        printf("Decompression error: size mismatch.\n");
        return 1;
    }
    
    int match = 1;
    for (size_t idx = 0; idx < input_len; idx++) {
        if (input_data[idx] != output_data[idx]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression verified: data matches original.\n");
    } else {
        printf("Decompression error: data mismatch.\n");
        return 1;
    }
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}