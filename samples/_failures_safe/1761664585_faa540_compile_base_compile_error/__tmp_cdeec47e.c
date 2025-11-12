//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size && out_index + 2 < output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < UINT8_MAX) {
            count++;
        }
        
        if (out_index + 2 >= output_size) {
            break;
        }
        
        output[out_index++] = (uint8_t)count;
        output[out_index++] = current;
        i += count;
    }
    
    return out_index;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_index < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        for (uint8_t j = 0; j < count && out_index < output_size; j++) {
            output[out_index++] = value;
        }
    }
    
    return out_index;
}

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || compress_func == NULL) {
        return result;
    }
    
    if (input_size > MAX_INPUT_SIZE) {
        return result;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
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
    
    if (input == NULL || input_size == 0 || decompress_func == NULL) {
        return result;
    }
    
    if (input_size > MAX_OUTPUT_SIZE) {
        return result;
    }
    
    uint8_t *output = malloc(MAX_INPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input, input_size, output, MAX_INPUT_SIZE);
    
    if (decompressed_size > 0 && decompressed_size <= MAX_INPUT_SIZE) {
        result.data = output;
        result.size = decompressed_size;
    } else {
        free(output);
    }
    
    return result;
}

void print_buffer(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_size = sizeof(test_data);
    
    printf("Original data (%zu bytes):\n", test_size);
    print_buffer(test_data, test_size);
    
    CompressFunc compress = run_length_encode;
    DecompressFunc decompress = run_length_decode;
    
    Buffer compressed = compress_data(test_data, test_size, compress);
    if (compressed.data != NULL) {
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_buffer(compressed.data, compressed.size);
        
        Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress);
        if (decompressed.data != NULL) {
            printf("Decompressed data (%zu bytes):\n", decompressed.size);
            print_buffer(decompressed.data, decompressed.size);
            
            if (decompressed.size == test_size && 
                memcmp(test_data,