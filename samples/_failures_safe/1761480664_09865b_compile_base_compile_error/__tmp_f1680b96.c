//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) <= 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

static int compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    VALIDATE_SIZE(input_size);
    
    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] & 0x03);
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_size = out_idx;
    return 0;
}

static int decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return -1;
    }
    if (input_size < HEADER_SIZE) {
        return -1;
    }
    
    CompressionHeader *header = (CompressionHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return -1;
    }
    if (header->original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t out_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_size && out_idx < header->original_size; i++) {
        uint8_t compressed_byte = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0 && out_idx < header->original_size; j--) {
            CHECK_BOUNDS(out_idx, MAX_INPUT_SIZE);
            output[out_idx++] = (compressed_byte >> (j * 2)) & 0x03;
        }
    }
    
    *output_size = out_idx;
    return 0;
}

static void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (hex string): ");
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        return EXIT_FAILURE;
    }
    
    size_t input_size = hex_len / 2;
    for (size_t i = 0; i < input_size; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char *endptr;
        long byte_val = strtol(hex_byte, &endptr, 16);
        if (endptr == hex_byte || *endptr != '\0' || byte_val < 0 || byte_val > 255) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    size_t compressed_size;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed data: ");
    print_hex(compressed_data, compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != 0) {
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_data, decompressed_data, input_size) !=