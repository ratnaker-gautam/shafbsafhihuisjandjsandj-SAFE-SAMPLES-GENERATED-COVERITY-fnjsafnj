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
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (!input || !output || input_len == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        
        uint8_t compressed_byte = 0;
        for (int j = 0; j < CHUNK_SIZE; j++) {
            if (i + j < input_len) {
                uint8_t current = input[i + j];
                uint8_t nibble = (current >> SHIFT_AMOUNT) & 0x0F;
                compressed_byte |= (nibble << (SHIFT_AMOUNT - j));
            }
        }
        output[output_idx++] = compressed_byte;
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (!input || !output || input_len == 0) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        uint8_t compressed = input[i];
        for (int j = 0; j < CHUNK_SIZE; j++) {
            CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
            
            uint8_t nibble = (compressed >> (SHIFT_AMOUNT - j)) & 0x0F;
            output[output_idx++] = (nibble << SHIFT_AMOUNT) | nibble;
        }
    }
    
    return output_idx;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes as hex digits, 2 chars per byte): ", MAX_INPUT_SIZE);
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (!fgets(hex_input, sizeof(hex_input), stdin)) {
        return EXIT_FAILURE;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0) {
        printf("Error: Hex input must have even number of characters\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = hex_len / 2;
    if (input_len > MAX_INPUT_SIZE) {
        printf("Error: Input too large\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char *endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) {
            printf("Error: Invalid hex digit\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_len = compress_data(input_data, input_len, compressed_data);
    if (compressed_len == 0) {
        printf("Error: Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data);
    if (decompressed_len == 0) {
        printf("Error: Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    printf("Compression ratio: %.2f:1\n", (double)input_len / compressed_len);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X", compressed_data[i]);
    }
    printf("\n");
    
    if (compare_data(input_data, decompressed_data, input_len)) {
        printf("Verification: Data matches after decompression\n");
    } else {
        printf("Error: Data mismatch after decompression