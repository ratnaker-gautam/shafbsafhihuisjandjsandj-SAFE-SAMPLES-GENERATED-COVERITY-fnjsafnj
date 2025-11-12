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
    uint8_t data[MAX_COMPRESSED_SIZE];
    size_t size;
} compressed_data_t;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t* input, size_t input_size, compressed_data_t* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
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
            if (out_index + 2 > MAX_COMPRESSED_SIZE) return 0;
            output->data[out_index++] = current;
            output->data[out_index++] = (uint8_t)run_length;
            i += run_length;
        } else {
            if (out_index + 1 > MAX_COMPRESSED_SIZE) return 0;
            output->data[out_index++] = current;
            i++;
        }
    }
    
    output->size = out_index;
    return out_index;
}

static size_t decompress_rle(const compressed_data_t* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0) return 0;
    if (output_size < MAX_INPUT_SIZE) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input->size) {
        uint8_t current = input->data[i++];
        
        if (i < input->size && input->data[i] >= MIN_COMPRESSIBLE_RUN) {
            uint8_t run_length = input->data[i++];
            
            if (out_index + run_length > output_size) return 0;
            
            for (uint8_t j = 0; j < run_length; j++) {
                output[out_index++] = current;
            }
        } else {
            if (out_index >= output_size) return 0;
            output[out_index++] = current;
        }
    }
    
    return out_index;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = 0;
    char* token = strtok(line, " \n");
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte_val;
        if (sscanf(token, "%02X", &byte_val) != 1) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        if (byte_val > 0xFF) {
            fprintf(stderr, "Byte value out of range: %s\n", token);
            return 1;
        }
        input_data[input_size++] = (uint8_t)byte_val;
        token = strtok(NULL, " \n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    print_hex(input_data, input_size);
    
    memset(&compressed, 0, sizeof(compressed));
    size_t compressed_size = compress_rle(input_data, input_size, &compressed);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    double ratio = (compressed_size * 100.0) /