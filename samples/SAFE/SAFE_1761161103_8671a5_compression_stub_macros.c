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

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
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
        
        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (out_idx + 3 > MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = ESCAPE_CHAR;
            output->data[out_idx++] = (uint8_t)run_length;
            output->data[out_idx++] = current;
        } else {
            if (out_idx + run_length > MAX_OUTPUT_SIZE) return 0;
            for (size_t j = 0; j < run_length; j++) {
                output->data[out_idx++] = current;
            }
        }
        
        i += run_length;
    }
    
    output->size = out_idx;
    return out_idx;
}

static size_t decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_size) return 0;
            
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (out_idx + run_length > MAX_OUTPUT_SIZE) return 0;
            
            for (uint8_t j = 0; j < run_length; j++) {
                output->data[out_idx++] = value;
            }
            
            i += 3;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = input[i++];
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t test_input[] = {0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x43, 0x43, 0xFF, 0xFF, 0xFF};
    size_t input_size = sizeof(test_input);
    
    printf("Original data (%zu bytes): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", test_input[i]);
    }
    printf("\n");
    
    Buffer compressed = {0};
    size_t compressed_size = compress_rle(test_input, input_size, &compressed);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    print_buffer_hex(&compressed);
    
    Buffer decompressed = {0};
    size_t decompressed_size = decompress_rle(compressed.data, compressed.size, &decompressed);
    
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    print_buffer_hex(&decompressed);
    
    if (decompressed_size != input_size) {
        fprintf(stderr, "Size mismatch: %zu vs %zu\n", decompressed_size, input_size);
        return EXIT_FAILURE;
    }
    
    if (memcmp(test_input, decompressed.data, input_size) != 0) {
        fprintf(stderr, "Data mismatch\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    printf("Compression ratio: %.2f%%\n", (100.0 * compressed_size) / input_size);
    
    return EXIT_SUCCESS;
}