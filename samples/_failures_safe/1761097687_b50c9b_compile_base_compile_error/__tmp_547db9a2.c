//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
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

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || chunk_size > CHUNK_SIZE) return 0;
    
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i += 2) {
        if (i + 1 < chunk_size) {
            uint8_t first = chunk[i] >> SHIFT_AMOUNT;
            uint8_t second = chunk[i + 1] >> SHIFT_AMOUNT;
            output[output_size++] = (first << SHIFT_AMOUNT) | second;
        } else {
            output[output_size++] = chunk[i] >> SHIFT_AMOUNT;
        }
    }
    return output_size;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || chunk_size > CHUNK_SIZE) return 0;
    
    size_t output_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        uint8_t compressed = chunk[i];
        output[output_size++] = (compressed >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        output[output_size++] = (compressed & (BYTE_MASK >> SHIFT_AMOUNT)) << SHIFT_AMOUNT;
    }
    return output_size;
}

static int compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        if (output->size + COMPRESSION_RATIO > MAX_OUTPUT_SIZE) return 0;
        
        size_t compressed_size = compress_chunk(input + total_processed, chunk_size, 
                                              output->data + output->size);
        if (compressed_size == 0) return 0;
        
        output->size += compressed_size;
        total_processed += chunk_size;
    }
    
    return 1;
}

static int decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size) {
        size_t remaining = input_size - total_processed;
        size_t chunk_size = (remaining < CHUNK_SIZE / COMPRESSION_RATIO) ? remaining : CHUNK_SIZE / COMPRESSION_RATIO;
        
        if (output->size + CHUNK_SIZE > MAX_OUTPUT_SIZE) return 0;
        
        size_t decompressed_size = decompress_chunk(input + total_processed, chunk_size,
                                                   output->data + output->size);
        if (decompressed_size == 0) return 0;
        
        output->size += decompressed_size;
        total_processed += chunk_size;
    }
    
    return 1;
}

static void print_buffer(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1) {
        fprintf(stderr, "Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (size_input <= 0 || size_input > MAX_INPUT_SIZE) {
        fprintf(stderr, "Invalid size range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes as hex values (00-FF):\n", size_input);
    for (int i = 0; i < size_input; i++) {
        unsigned int byte_val;
        if (scanf("%02x", &byte_val) != 1) {
            fprintf(stderr, "Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if