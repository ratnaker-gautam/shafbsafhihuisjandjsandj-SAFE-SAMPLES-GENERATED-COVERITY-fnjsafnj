//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(size, max) if (size > max || size < 0) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (!input || !output || input_size == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        if (output_index >= MAX_OUTPUT_SIZE - 1) {
            break;
        }
        
        uint8_t compressed_byte = 0;
        for (int j = 0; j < CHUNK_SIZE; j++) {
            if (i + j < input_size) {
                uint8_t current_byte = input[i + j];
                uint8_t shifted = (current_byte >> SHIFT_AMOUNT) & 0x0F;
                compressed_byte |= (shifted << (j * 1));
            }
        }
        output[output_index++] = compressed_byte;
    }
    
    return output_index;
}

static size_t decompress_data(const uint8_t* input, size_t input_size, uint8_t* output) {
    if (!input || !output || input_size == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    for (size_t i = 0; i < input_size; i++) {
        if (output_index >= MAX_OUTPUT_SIZE - CHUNK_SIZE) {
            break;
        }
        
        uint8_t compressed_byte = input[i];
        for (int j = 0; j < CHUNK_SIZE; j++) {
            if (output_index < MAX_OUTPUT_SIZE) {
                uint8_t shifted = (compressed_byte >> (j * 1)) & 0x0F;
                output[output_index++] = (shifted << SHIFT_AMOUNT) | shifted;
            }
        }
    }
    
    return output_index;
}

static int compare_data(const uint8_t* data1, const uint8_t* data2, size_t size) {
    if (!data1 || !data2) {
        return 0;
    }
    
    for (size_t i = 0; i < size; i++) {
        if (data1[i] != data2[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    CHECK_BOUNDS(input_size, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255): ", input_size);
    for (int i = 0; i < input_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %d bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f:1\n", (float)input_size / compressed_size);
    
    int matches = compare_data(input_data, decompressed_data, input_size);
    printf("Data integrity: %s\n", matches ? "PASS" : "FAIL");
    
    if (matches) {
        printf("Compression test successful\n");
    } else {
        printf("Compression test failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}