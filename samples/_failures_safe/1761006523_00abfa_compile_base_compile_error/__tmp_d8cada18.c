//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MARKER 0xFF
#define MAX_RUN_LENGTH 255

typedef struct {
    uint8_t data[MAX_INPUT_SIZE * 2];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && 
               input[i + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length > 3 || current == COMPRESSED_MARKER) {
            if (output->size + 3 > sizeof(output->data)) return 0;
            output->data[output->size++] = COMPRESSED_MARKER;
            output->data[output->size++] = (uint8_t)run_length;
            output->data[output->size++] = current;
        } else {
            if (output->size + run_length > sizeof(output->data)) return 0;
            for (size_t j = 0; j < run_length; j++) {
                output->data[output->size++] = current;
            }
        }
        
        i += run_length;
    }
    
    return output->size;
}

static size_t decompress_rle(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (input->data[in_idx] == COMPRESSED_MARKER) {
            if (in_idx + 2 >= input->size) return 0;
            
            uint8_t run_length = input->data[in_idx + 1];
            uint8_t value = input->data[in_idx + 2];
            
            if (out_idx + run_length > output_size) return 0;
            
            for (uint8_t j = 0; j < run_length; j++) {
                output[out_idx++] = value;
            }
            
            in_idx += 3;
        } else {
            if (out_idx >= output_size) return 0;
            output[out_idx++] = input->data[in_idx++];
        }
    }
    
    return out_idx;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    Buffer compressed;
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size_input <= 0 || size_input > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    
    size_t data_size = (size_t)size_input;
    
    printf("Enter %zu bytes of data (0-255):\n", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int byte_input;
        if (scanf("%d", &byte_input) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        if (byte_input < 0 || byte_input > 255) {
            printf("Byte value out of range\n");
            return 1;
        }
        original_data[i] = (uint8_t)byte_input;
    }
    
    printf("\nOriginal data (%zu bytes):\n", data_size);
    print_hex(original_data, data_size);
    
    size_t compressed_size = compress_rle(original_data, data_size, &compressed);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    size_t decompressed_size = decompress_rle(&compressed, decompressed, sizeof(decompressed));
    if (decompressed_size == 0 || decompressed_size != data_size) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed, decompressed_size);
    
    if