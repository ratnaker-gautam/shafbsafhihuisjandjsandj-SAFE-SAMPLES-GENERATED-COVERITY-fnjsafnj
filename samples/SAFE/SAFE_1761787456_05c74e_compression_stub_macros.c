//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
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
    
    size_t out_idx = 0;
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
            if (out_idx + 2 >= sizeof(output->data)) return 0;
            output->data[out_idx++] = COMPRESSED_MARKER;
            output->data[out_idx++] = (uint8_t)run_length;
            output->data[out_idx++] = current;
        } else {
            if (out_idx + run_length >= sizeof(output->data)) return 0;
            for (size_t j = 0; j < run_length; j++) {
                output->data[out_idx++] = current;
            }
        }
        
        i += run_length;
    }
    
    output->size = out_idx;
    return out_idx;
}

static size_t decompress_rle(const Buffer* input, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (output_size < MAX_INPUT_SIZE) return 0;
    
    size_t in_idx = 0;
    size_t out_idx = 0;
    
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
    uint8_t original[] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x44, 0x45, 0x45, 0x45};
    size_t original_size = sizeof(original);
    
    printf("Original data (%zu bytes):\n", original_size);
    print_hex(original, original_size);
    
    Buffer compressed = {0};
    size_t compressed_size = compress_rle(original, original_size, &compressed);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    uint8_t decompressed[MAX_INPUT_SIZE] = {0};
    size_t decompressed_size = decompress_rle(&compressed, decompressed, sizeof(decompressed));
    
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed, decompressed_size);
    
    if (decompressed_size != original_size) {
        fprintf(stderr, "Size mismatch: %zu vs %zu\n", decompressed_size, original_size);
        return EXIT_FAILURE;
    }
    
    if (memcmp(original, decompressed, original_size) != 0) {
        fprintf(stderr, "Data mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    return EXIT_SUCCESS;
}