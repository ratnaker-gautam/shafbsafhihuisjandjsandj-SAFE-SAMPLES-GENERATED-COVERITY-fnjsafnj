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
        if (decompressed_size >= MAX_OUTPUT_SIZE) break;
        output[decompressed_size++] = chunk[i] ^ MASK_BYTE;
    }
    return decompressed_size;
}

static int perform_compression(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    const uint8_t* current = input;
    
    while (remaining > 0 && output->size < MAX_OUTPUT_SIZE) {
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        size_t compressed = compress_chunk(current, chunk_size, 
                                         output->data + output->size);
        
        if (compressed == 0) break;
        
        output->size += compressed;
        current += chunk_size;
        remaining -= chunk_size;
    }
    
    return (output->size > 0);
}

static int perform_decompression(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    output->size = 0;
    size_t remaining = input_size;
    const uint8_t* current = input;
    
    while (remaining > 0 && output->size < MAX_OUTPUT_SIZE) {
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        size_t decompressed = decompress_chunk(current, chunk_size, 
                                              output->data + output->size);
        
        if (decompressed == 0) break;
        
        output->size += decompressed;
        current += chunk_size;
        remaining -= chunk_size;
    }
    
    return (output->size > 0);
}

static void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        if (i > 0 && i % 16 == 0) printf("\n");
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char input_line[MAX_INPUT_SIZE + 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, input_line, input_len);
    size_t input_size = input_len;
    
    printf("Original data (%zu bytes):\n", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    if (!perform_compression(input_data, input_size, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer(&compressed);
    
    if (!perform_decompression(compressed.data, compressed.size, &decompressed)) {