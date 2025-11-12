//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define BYTE_MASK 0xFF
#define VALIDATE_PTR(ptr) if (!ptr) { return EXIT_FAILURE; }

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int compress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        if (i + 1 < len) {
            uint8_t first = input[i] & BYTE_MASK;
            uint8_t second = input[i + 1] & BYTE_MASK;
            output[out_idx++] = (first >> 4) | ((second >> 4) << 4);
        } else {
            output[out_idx++] = input[i] & BYTE_MASK;
        }
    }
    return out_idx;
}

int decompress_chunk(const uint8_t* input, size_t len, uint8_t* output) {
    if (len == 0 || len > CHUNK_SIZE) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t val = input[i] & BYTE_MASK;
        output[out_idx++] = (val & 0x0F) << 4;
        if (out_idx < MAX_OUTPUT_SIZE) {
            output[out_idx++] = (val >> 4) << 4;
        }
    }
    return out_idx;
}

int perform_compression(const uint8_t* input, size_t input_len, CompressedData* result) {
    if (!input || input_len == 0 || input_len > MAX_INPUT_SIZE || !result) return 0;
    result->size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        int compressed_size = compress_chunk(input + i, chunk_len, result->data + result->size);
        if (compressed_size <= 0 || result->size + compressed_size > MAX_OUTPUT_SIZE) return 0;
        result->size += compressed_size;
    }
    return 1;
}

int perform_decompression(const CompressedData* compressed, uint8_t* output, size_t* output_len) {
    if (!compressed || compressed->size == 0 || compressed->size > MAX_OUTPUT_SIZE || !output || !output_len) return 0;
    *output_len = 0;
    for (size_t i = 0; i < compressed->size; i += CHUNK_SIZE) {
        size_t chunk_len = (compressed->size - i) < CHUNK_SIZE ? (compressed->size - i) : CHUNK_SIZE;
        int decompressed_size = decompress_chunk(compressed->data + i, chunk_len, output + *output_len);
        if (decompressed_size <= 0 || *output_len + decompressed_size > MAX_INPUT_SIZE) return 0;
        *output_len += decompressed_size;
    }
    return 1;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    CompressedData compressed;
    size_t input_len = 0;
    size_t output_len = 0;
    
    printf("Enter input data (up to %d bytes as hex digits, 2 chars per byte): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (!fgets(hex_input, sizeof(hex_input), stdin)) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        printf("Invalid hex input length\n");
        return EXIT_FAILURE;
    }
    
    input_len = hex_len / 2;
    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char* endptr;
        long val = strtol(hex_byte, &endptr, 16);
        if (endptr == hex_byte || *endptr != '\0' || val < 0 || val > 255) {
            printf("Invalid hex byte at position %zu\n", i);
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)val;
    }
    
    printf("Original data (%zu bytes): ", input_len);
    for (size_t i = 0; i < input_len