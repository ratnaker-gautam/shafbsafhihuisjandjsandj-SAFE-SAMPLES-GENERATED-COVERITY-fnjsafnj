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
#define MASK_BYTE 0xAA

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

static size_t compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || output == NULL) return 0;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        if (compressed_size >= MAX_OUTPUT_SIZE) break;
        output[compressed_size++] = chunk[i] ^ MASK_BYTE;
    }
    return compressed_size;
}

static size_t decompress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    if (chunk_size == 0 || output == NULL) return 0;
    
    size_t decompressed_size = 0;
    for (size_t i = 0; i < chunk_size; i++) {
        if (decompressed_size >= MAX_INPUT_SIZE) break;
        output[decompressed_size++] = chunk[i] ^ MASK_BYTE;
    }
    return decompressed_size;
}

static int perform_compression(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size && output->size < MAX_OUTPUT_SIZE) {
        size_t remaining = input_size - total_processed;
        size_t current_chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        size_t compressed_size = compress_chunk(input + total_processed, 
                                              current_chunk_size, 
                                              output->data + output->size);
        
        if (compressed_size == 0) break;
        
        output->size += compressed_size;
        total_processed += current_chunk_size;
    }
    
    return (total_processed == input_size);
}

static int perform_decompression(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t total_processed = 0;
    
    while (total_processed < input_size && output->size < MAX_INPUT_SIZE) {
        size_t remaining = input_size - total_processed;
        size_t current_chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        size_t decompressed_size = decompress_chunk(input + total_processed, 
                                                   current_chunk_size, 
                                                   output->data + output->size);
        
        if (decompressed_size == 0) break;
        
        output->size += decompressed_size;
        total_processed += current_chunk_size;
    }
    
    return (total_processed == input_size);
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    
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
    
    printf("Enter data to compress (hex bytes, space separated): ");
    
    size_t input_size = 0;
    while (input_size < MAX_INPUT_SIZE) {
        int byte;
        if (scanf("%02x", &byte) != 1) break;
        if (byte < 0 || byte > 255) break;
        input_data[input_size++] = (uint8_t)byte;
        
        int c = getchar();
        if (c == EOF || c == '\n') break;
    }
    
    if (input_size == 0) {
        printf("No valid input provided.\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    Buffer original = {0};
    memcpy(original.data, input_data, input_size);
    original.size = input_size;
    print_buffer(&original);
    
    printf("\nCompressing...\n");
    if (!perform_compression(input_data, input_size, &compressed)) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer(&compressed);
    
    printf("\nDecompressing...\n");
    if (!perform_de