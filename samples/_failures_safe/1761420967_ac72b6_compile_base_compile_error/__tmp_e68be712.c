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
#define BYTE_MASK 0xFF

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

static void compress_chunk(const uint8_t* chunk, size_t chunk_size, uint8_t* output) {
    uint16_t compressed = 0;
    for (size_t i = 0; i < chunk_size && i < CHUNK_SIZE; i++) {
        compressed = (compressed << 2) | (chunk[i] >> 6);
    }
    output[0] = (compressed >> 8) & BYTE_MASK;
    output[1] = compressed & BYTE_MASK;
}

static size_t compress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    size_t output_size = 0;
    size_t chunks = (input_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    
    if (chunks * 2 > MAX_OUTPUT_SIZE) return 0;
    
    for (size_t i = 0; i < chunks; i++) {
        size_t chunk_start = i * CHUNK_SIZE;
        size_t chunk_size = (input_size - chunk_start) < CHUNK_SIZE ? 
                           (input_size - chunk_start) : CHUNK_SIZE;
        
        compress_chunk(&input[chunk_start], chunk_size, &output->data[output_size]);
        output_size += 2;
    }
    
    output->size = output_size;
    return output_size;
}

static void decompress_chunk(const uint8_t* compressed, uint8_t* output, size_t chunk_size) {
    uint16_t value = (compressed[0] << 8) | compressed[1];
    for (size_t i = 0; i < chunk_size && i < CHUNK_SIZE; i++) {
        output[i] = ((value >> (2 * (CHUNK_SIZE - 1 - i))) & 0x03) << 6;
    }
}

static size_t decompress_data(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t output_size = (input_size / 2) * CHUNK_SIZE;
    if (output_size > MAX_OUTPUT_SIZE) return 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        size_t chunk_index = i / 2;
        size_t chunk_start = chunk_index * CHUNK_SIZE;
        size_t actual_chunk_size = (output_size - chunk_start) < CHUNK_SIZE ? 
                                  (output_size - chunk_start) : CHUNK_SIZE;
        
        decompress_chunk(&input[i], &output->data[chunk_start], actual_chunk_size);
    }
    
    output->size = output_size;
    return output_size;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size <= 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of range\n");
        return 1;
    }
    
    printf("Enter %d bytes as hex (e.g., A1 B2 C3): ", input_size);
    for (int i = 0; i < input_size; i++) {
        unsigned int byte;
        if (scanf("%02X", &byte) != 1) {
            printf("Invalid byte input\n");
            return 1;
        }
        if (byte > 0xFF) {
            printf("Byte value out of range\n");
            return 1;
        }
        input_data[i] = (uint8_t)byte;
    }
    
    size_t comp_size = compress_data(input_data, input_size, &compressed);
    if (comp_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", comp_size);
    print_hex(compressed.data, comp_size);
    
    size_t decomp_size = decompress_data(compressed.data, comp_size, &decompressed);
    if (decomp_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decomp