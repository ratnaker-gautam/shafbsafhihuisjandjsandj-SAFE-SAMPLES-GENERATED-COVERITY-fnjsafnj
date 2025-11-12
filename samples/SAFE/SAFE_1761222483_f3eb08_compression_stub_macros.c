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

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(len, max) if ((len) > (max) || (len) < 0) { return EXIT_FAILURE; }
#define VALIDATE_INPUT(input, len) if ((input) == NULL || (len) <= 0) { return EXIT_FAILURE; }

static size_t stub_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL || output_max < input_len / COMPRESSION_RATIO) {
        return 0;
    }
    size_t output_len = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        if (output_len >= output_max) {
            return 0;
        }
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE && (i + j) < input_len; j++) {
            compressed_byte ^= (input[i + j] & MASK_BYTE) >> (j % 8);
        }
        output[output_len++] = compressed_byte;
    }
    return output_len;
}

static size_t stub_decompress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    VALIDATE_INPUT(input, input_len);
    if (output == NULL || output_max < input_len * COMPRESSION_RATIO) {
        return 0;
    }
    size_t output_len = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (output_len + CHUNK_SIZE > output_max) {
            return 0;
        }
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            output[output_len++] = (input[i] << (j % 8)) ^ MASK_BYTE;
        }
    }
    return output_len;
}

static int process_compression(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes as hex digits, e.g., A1B2C3): ", MAX_INPUT_SIZE);
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
    size_t input_len = hex_len / 2;
    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = { hex_input[i * 2], hex_input[i * 2 + 1], '\0' };
        char *endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_len = stub_compress(input_data, input_len, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        return EXIT_FAILURE;
    }
    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X", compressed_data[i]);
    }
    printf("\n");
    
    size_t decompressed_len = stub_decompress(compressed_data, compressed_len, decompressed_data, MAX_OUTPUT_SIZE);
    if (decompressed_len == 0 || decompressed_len != input_len) {
        return EXIT_FAILURE;
    }
    if (memcmp(input_data, decompressed_data, input_len) != 0) {
        return EXIT_FAILURE;
    }
    printf("Decompressed data verified successfully.\n");
    return EXIT_SUCCESS;
}

int main(void) {
    return process_compression();
}