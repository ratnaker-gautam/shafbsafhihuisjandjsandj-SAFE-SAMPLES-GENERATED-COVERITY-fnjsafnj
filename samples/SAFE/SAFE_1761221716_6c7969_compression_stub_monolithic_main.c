//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
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
    
    while (i < input->size - 1 && *output_size < MAX_INPUT_SIZE) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            break;
        }
        
        for (uint8_t j = 0; j < count && *output_size < MAX_INPUT_SIZE; j++) {
            output[(*output_size)++] = value;
        }
        
        i += 2;
    }
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }
    
    if (input_size == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    compress_rle(input_data, input_size, &compressed);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input_size - compressed.size) / input_size * 100);
        
        printf("Compressed data: ");
        for (size_t i = 0; i < compressed.size; i++) {
            printf("%02X ", compressed.data[i]);
        }
        printf("\n");
        
        decompress_rle(&compressed, decompressed, &decompressed_size);
        
        if (decompressed_size == input_size && 
            memcmp(input_data, decompressed, input_size) == 0) {
            printf("Decompression successful: %s\n", decompressed);
        } else {
            printf("Decompression failed\n");
            return 1;
        }
    } else {
        printf("Compression failed\n");
        return 1;
    }
    
    return 0;
}