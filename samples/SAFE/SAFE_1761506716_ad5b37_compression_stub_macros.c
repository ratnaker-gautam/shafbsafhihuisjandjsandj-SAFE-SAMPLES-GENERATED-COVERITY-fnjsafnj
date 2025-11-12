//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define CHUNK_SIZE 16
#define COMPRESSION_THRESHOLD 4
#define RUN_LENGTH_MASK 0x7F
#define MAX_RUN_LENGTH 127

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
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t run_length = 1;
        
        while (in_idx + run_length < input_size && 
               input[in_idx + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (out_idx + 2 > MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = RUN_LENGTH_MASK | (uint8_t)run_length;
            output->data[out_idx++] = current;
            in_idx += run_length;
        } else {
            if (out_idx + run_length > MAX_OUTPUT_SIZE) return 0;
            for (size_t i = 0; i < run_length; i++) {
                output->data[out_idx++] = input[in_idx++];
            }
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static size_t decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t byte = input[in_idx++];
        
        if (byte & RUN_LENGTH_MASK) {
            size_t run_length = byte & RUN_LENGTH_MASK;
            if (in_idx >= input_size) return 0;
            uint8_t value = input[in_idx++];
            
            if (out_idx + run_length > MAX_OUTPUT_SIZE) return 0;
            for (size_t i = 0; i < run_length; i++) {
                output->data[out_idx++] = value;
            }
        } else {
            if (out_idx + 1 > MAX_OUTPUT_SIZE) return 0;
            output->data[out_idx++] = byte;
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % CHUNK_SIZE == 0) printf(" ");
    }
    printf("\n");
}

int main(void) {
    uint8_t test_input[] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x44, 0x44, 0x44, 0x44};
    size_t input_size = sizeof(test_input);
    
    if (!validate_input(test_input, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    Buffer original = {0};
    memcpy(original.data, test_input, input_size);
    original.size = input_size;
    print_buffer_hex(&original);
    
    Buffer compressed = {0};
    size_t compressed_size = compress_rle(test_input, input_size, &compressed);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    print_buffer_hex(&compressed);
    
    Buffer decompressed = {0};
    size_t decompressed_size = decompress_rle(compressed.data, compressed_size, &decompressed);
    if (decompressed_size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    print_buffer_hex(&decompressed);
    
    if (decompressed_size != input_size || memcmp(test_input, decompressed.data, input_size) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression successful: %zu -> %zu bytes\n", input_size, compressed_size);
    return EXIT_SUCCESS;
}