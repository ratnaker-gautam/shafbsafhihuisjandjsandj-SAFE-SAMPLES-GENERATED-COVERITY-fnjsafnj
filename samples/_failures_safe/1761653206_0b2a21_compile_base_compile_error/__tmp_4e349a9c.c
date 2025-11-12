//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t value = input[i++];
        uint8_t count = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || compress_func == NULL) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t compressed_size = compress_func(input, input_size, output, MAX_OUTPUT_SIZE);
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.data = output;
        result.size = compressed_size;
    } else {
        free(output);
    }
    
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || decompress_func == NULL) return result;
    
    uint8_t *output = malloc(MAX_INPUT_SIZE);
    if (output == NULL) return result;
    
    size_t decompressed_size = decompress_func(input, input_size, output, MAX_INPUT_SIZE);
    if (decompressed_size > 0 && decompressed_size <= MAX_INPUT_SIZE) {
        result.data = output;
        result.size = decompressed_size;
    } else {
        free(output);
    }
    
    return result;
}

void process_compression_operation(int operation) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1 || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1 || byte > 255) {
            printf("Invalid byte input\n");
            while (getchar() != '\n');
            return;
        }
        input_data[i] = (uint8_t)byte;
    }
    
    CompressFunc compress = run_length_encode;
    DecompressFunc decompress = run_length_decode;
    
    if (operation == 1) {
        Buffer compressed = compress_data(input_data, input_size, compress);
        if (compressed.data != NULL) {
            printf("Compressed data (%zu bytes): ", compressed.size);
            for (size_t i = 0; i < compressed.size; i++) {
                printf("%02x", compressed.data[i]);
            }
            printf("\n");
            free(compressed.data);
        } else {
            printf("Compression failed\n");
        }
    } else {
        if (input_size % 2 != 0) {
            printf("Invalid compressed data format\n");
            return;
        }
        
        Buffer decompressed = decompress_data(input_data, input_size, decompress);
        if (decompressed.data != NULL) {
            printf("Decompressed data (%zu bytes): ", decompressed.size);
            for (size_t i = 0; i < decompressed.size; i++) {
                printf("%02x", decompressed.data[i]);
            }
            printf("\n");
            free(decompressed.data);
        } else {