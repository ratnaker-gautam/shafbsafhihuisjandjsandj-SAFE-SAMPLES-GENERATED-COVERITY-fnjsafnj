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

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        
        i += count;
    }
    
    return 0;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input->size == 0) {
        return -1;
    }
    
    if (input->size % 2 != 0) {
        return -1;
    }
    
    *output_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (*output_size + count > MAX_INPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[*output_size] = value;
            (*output_size)++;
        }
    }
    
    return 0;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t input_size = 0;
    size_t decompressed_size = 0;
    
    printf("Enter input data (up to %d bytes as hex values, space separated): ", MAX_INPUT_SIZE);
    
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        unsigned int value;
        if (scanf("%2x", &value) != 1) {
            break;
        }
        if (value > 0xFF) {
            printf("Invalid hex value\n");
            return 1;
        }
        input_data[input_size] = (uint8_t)value;
        input_size++;
        if (getchar() == '\n') {
            break;
        }
    }
    
    if (input_size == 0) {
        printf("No input data provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    if (compress_rle(input_data, input_size, &compressed) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100.0f);
    
    if (decompress_rle(&compressed, output_data, &decompressed_size) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (decompressed_size != input_size) {
        printf("Decompression size mismatch\n");
        return 1;
    }
    
    if (memcmp(input_data, output_data, input_size) != 0) {
        printf("Decompression data mismatch\n");
        return 1;
    }
    
    printf("Decompression successful - data verified\n");
    
    return 0;
}