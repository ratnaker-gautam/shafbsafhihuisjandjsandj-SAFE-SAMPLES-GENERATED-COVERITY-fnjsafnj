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
    if (input->size == 0) return 0;
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input->size) {
        if (input->data[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input->size) return 0;
            
            uint8_t run_length = input->data[i + 1];
            uint8_t value = input->data[i + 2];
            
            if (output_index + run_length > output_size) return 0;
            
            for (uint8_t j = 0; j < run_length; j++) {
                output[output_index++] = value;
            }
            
            i += 3;
        } else {
            if (output_index >= output_size) return 0;
            output[output_index++] = input->data[i++];
        }
    }
    
    return output_index;
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
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    Buffer compressed;
    
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
    
    printf("Enter %d bytes (hex values 00-FF): ", size_input);
    for (int i = 0; i < size_input; i++) {
        unsigned int byte;
        if (scanf("%02x", &byte) != 1) {
            printf("Invalid byte\n");
            return 1;
        }
        if (byte > 0xFF) {
            printf("Byte out of range\n");
            return 1;
        }
        original_data[i] = (uint8_t)byte;
    }
    
    size_t compressed_size = compress_rle(original_data, size_input, &compressed);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    size_t decompressed_size = decompress_rle(&compressed, decompressed_data, sizeof(decompressed_data));
    if (decompressed_size == 0 || decompressed_size != (size_t)size_input) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (memcmp(original_data, decompressed_data, size_input) != 0) {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Original size: %d bytes\n", size_input);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / size_input * 100.0f);
    
    printf("\nCompressed data:\