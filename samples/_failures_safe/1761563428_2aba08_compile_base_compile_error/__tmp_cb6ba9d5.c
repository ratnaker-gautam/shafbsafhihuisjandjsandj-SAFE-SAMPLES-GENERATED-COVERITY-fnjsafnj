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
        
        if (compressed_size == 0) return 0;
        
        output->size += compressed_size;
        total_processed += current_chunk_size;
    }
    
    return 1;
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
        
        if (decompressed_size == 0) return 0;
        
        output->size += decompressed_size;
        total_processed += current_chunk_size;
    }
    
    return 1;
}

static void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

static int read_input_string(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL || max_size == 0) return 0;
    
    printf("Enter text to compress (max %zu characters): ", max_size - 1);
    fflush(stdout);
    
    if (fgets((char*)buffer, max_size, stdin) == NULL) return 0;
    
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? (int)len : 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    int input_size = read_input_string(input_buffer, MAX_INPUT_SIZE);
    if (input_size <= 0) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("\nOriginal data (%d bytes):\n", input_size);
    for (int i = 0; i < input_size; i++) {
        printf("%c", input_buffer[i]);
    }
    printf("\n");
    
    if (!perform_compression(input_buffer, input_size, &compressed)) {
        fprintf(stderr, "Error: Compression failed\n");
        return EXIT