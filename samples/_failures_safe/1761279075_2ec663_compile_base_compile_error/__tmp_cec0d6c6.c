//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR 0xFF
#define MAX_RUN_LENGTH 255

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    if (input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_idx + 2 > output_max) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_len && input[i + run_length] == current && run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (output_idx + 3 > output_max) {
                return 0;
            }
            output[output_idx++] = ESCAPE_CHAR;
            output[output_idx++] = (uint8_t)run_length;
            output[output_idx++] = current;
            i += run_length;
        } else {
            output[output_idx++] = current;
            i++;
        }
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_idx >= output_max) {
            return 0;
        }
        
        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_len) {
                return 0;
            }
            
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (output_idx + run_length > output_max) {
                return 0;
            }
            
            for (uint8_t j = 0; j < run_length; j++) {
                output[output_idx++] = value;
            }
            
            i += 3;
        } else {
            output[output_idx++] = input[i++];
        }
    }
    
    return output_idx;
}

static void print_hex(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

static int verify_decompression(const uint8_t *original, const uint8_t *decompressed, size_t len) {
    if (original == NULL || decompressed == NULL) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (original[i] != decompressed[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter input data size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255) separated by spaces:\n", input_size);
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
    
    printf("\nOriginal data (%zu bytes):\n", (size_t)input_size);
    print_hex(input_data, input_size);
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed_data, compressed_size);
    
    size_t decompressed_size = decompress_data(compressed_data