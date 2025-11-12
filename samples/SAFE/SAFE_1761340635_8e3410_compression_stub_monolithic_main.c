//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int compress_rle(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
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
            return -1;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const CompressedData* compressed, uint8_t* output, size_t output_size) {
    if (compressed == NULL || output == NULL || compressed->size == 0) {
        return -1;
    }
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        if (i + 1 >= compressed->size) {
            return -1;
        }
        
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (output_index + count > output_size) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_index++] = value;
        }
    }
    
    return 0;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_length = strlen((char*)input_data);
    if (input_length > 0 && input_data[input_length - 1] == '\n') {
        input_data[--input_length] = '\0';
    }
    
    if (input_length == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original data: %s\n", input_data);
    printf("Original size: %zu bytes\n", input_length);
    
    if (compress_rle(input_data, input_length, &compressed) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_length - compressed.size) / input_length * 100.0f);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    if (decompress_rle(&compressed, decompressed_data, MAX_INPUT_SIZE) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    decompressed_data[input_length] = '\0';
    
    if (memcmp(input_data, decompressed_data, input_length) != 0) {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompressed data: %s\n", decompressed_data);
    printf("Decompression successful\n");
    
    return 0;
}