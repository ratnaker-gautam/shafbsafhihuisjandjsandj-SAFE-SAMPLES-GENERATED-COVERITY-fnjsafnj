//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER)
#define RUN_LENGTH_MAX 255
#define MIN_COMPRESSIBLE_RUN 3

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

Buffer compress_run_length(const uint8_t *input, size_t input_size) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return result;
    }
    
    result.data = malloc(MAX_COMPRESSED_SIZE);
    if (result.data == NULL) {
        return result;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && 
               input[i + run_length] == current && 
               run_length < RUN_LENGTH_MAX) {
            run_length++;
        }
        
        if (run_length >= MIN_COMPRESSIBLE_RUN) {
            if (out_index + 2 > MAX_COMPRESSED_SIZE) {
                free(result.data);
                result.data = NULL;
                return result;
            }
            result.data[out_index++] = current;
            result.data[out_index++] = (uint8_t)run_length;
            i += run_length;
        } else {
            if (out_index + 1 > MAX_COMPRESSED_SIZE) {
                free(result.data);
                result.data = NULL;
                return result;
            }
            result.data[out_index++] = current;
            i++;
        }
    }
    
    result.size = out_index;
    return result;
}

Buffer decompress_run_length(const uint8_t *compressed, size_t compressed_size) {
    Buffer result = {NULL, 0};
    if (compressed == NULL || compressed_size == 0 || compressed_size > MAX_COMPRESSED_SIZE) {
        return result;
    }
    
    result.data = malloc(MAX_INPUT_SIZE);
    if (result.data == NULL) {
        return result;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < compressed_size) {
        if (i + 1 < compressed_size && compressed[i + 1] >= MIN_COMPRESSIBLE_RUN) {
            uint8_t value = compressed[i];
            uint8_t count = compressed[i + 1];
            
            if (out_index + count > MAX_INPUT_SIZE) {
                free(result.data);
                result.data = NULL;
                return result;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                result.data[out_index++] = value;
            }
            i += 2;
        } else {
            if (out_index + 1 > MAX_INPUT_SIZE) {
                free(result.data);
                result.data = NULL;
                return result;
            }
            result.data[out_index++] = compressed[i];
            i++;
        }
    }
    
    result.size = out_index;
    return result;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int temp;
        if (scanf("%u", &temp) != 1) {
            printf("Invalid input data\n");
            return 1;
        }
        if (temp > UINT8_MAX) {
            printf("Value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)temp;
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < input_size; i++) {
        printf("%u ", input_data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_run_length(input_data, input_size);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%u ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_run_length(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed size: %zu\n", decompressed.size);
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%u ", decompressed.data[i]);
    }
    printf("\n");