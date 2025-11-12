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

static Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer result = {0};
    if (!validate_input(input, input_size)) return result;
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t remaining = input_size - i;
        size_t chunk_size = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        
        uint8_t compressed_chunk[CHUNK_SIZE];
        size_t compressed_size = compress_chunk(input + i, chunk_size, compressed_chunk);
        
        if (total_compressed + compressed_size > MAX_OUTPUT_SIZE) break;
        
        memcpy(result.data + total_compressed, compressed_chunk, compressed_size);
        total_compressed += compressed_size;
    }
    
    result.size = total_compressed;
    return result;
}

static void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        if (i > 0 && i % 16 == 0) printf("\n");
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes):\n", MAX_INPUT_SIZE);
    
    int c;
    while (input_size < MAX_INPUT_SIZE) {
        c = getchar();
        if (c == EOF || c == '\n') break;
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return EXIT_FAILURE;
    }
    
    if (input_size > MAX_INPUT_SIZE) {
        printf("Input too large.\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed = compress_data(input_data, input_size);
    
    if (compressed.size == 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f:1\n", (float)input_size / compressed.size);
    printf("Compressed data (hex):\n");
    print_hex(compressed.data, compressed.size);
    
    return EXIT_SUCCESS;
}