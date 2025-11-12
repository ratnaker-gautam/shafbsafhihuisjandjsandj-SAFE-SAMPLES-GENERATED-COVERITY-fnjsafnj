//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || compress == NULL) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t compressed_size = compress(input, input_size, output, MAX_OUTPUT_SIZE);
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.data = malloc(compressed_size);
        if (result.data != NULL) {
            memcpy(result.data, output, compressed_size);
            result.size = compressed_size;
        }
    }
    
    free(output);
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || decompress == NULL) return result;
    
    uint8_t *output = malloc(MAX_INPUT_SIZE);
    if (output == NULL) return result;
    
    size_t decompressed_size = decompress(input, input_size, output, MAX_INPUT_SIZE);
    if (decompressed_size > 0 && decompressed_size <= MAX_INPUT_SIZE) {
        result.data = malloc(decompressed_size);
        if (result.data != NULL) {
            memcpy(result.data, output, decompressed_size);
            result.size = decompressed_size;
        }
    }
    
    free(output);
    return result;
}

void process_compression_demo(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) break;
        if (byte > 0xFF) break;
        input_data[i] = (uint8_t)byte;
        input_size++;
        if (getchar() == '\n') break;
    }
    
    if (input_size == 0) {
        printf("No valid input provided.\n");
        return;
    }
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    Buffer compressed = compress_data(input_data, input_size, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        return;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress_func);
    if (decompressed.data == NULL) {
        printf("Decompression failed.\n");
        free(compressed.data);
        return;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%02X ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input_size && memcmp(input