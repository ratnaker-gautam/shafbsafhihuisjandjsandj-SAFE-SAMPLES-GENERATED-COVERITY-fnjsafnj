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
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

size_t compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    if (output_size < HEADER_SIZE + (input_size + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO) return 0;

    CompressionHeader* header = (CompressionHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] >> 6);
        }
        CHECK_BOUNDS(output_idx, output_size);
        output[output_idx++] = compressed_byte;
    }
    return output_idx;
}

size_t decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size < HEADER_SIZE) return 0;

    CompressionHeader* header = (CompressionHeader*)input;
    if (header->magic != MAGIC_NUMBER) return 0;
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) return 0;
    if (output_size < header->original_size) return 0;

    size_t input_idx = HEADER_SIZE;
    size_t output_idx = 0;
    while (output_idx < header->original_size) {
        if (input_idx >= input_size) return 0;
        uint8_t compressed_byte = input[input_idx++];
        for (int j = COMPRESSION_RATIO - 1; j >= 0 && output_idx < header->original_size; j--) {
            CHECK_BOUNDS(output_idx, output_size);
            output[output_idx++] = (compressed_byte >> (j * 2)) & 0x03;
        }
    }
    return output_idx;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    if (!fgets((char*)input_data, sizeof(input_data), stdin)) {
        return EXIT_FAILURE;
    }

    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }
    VALIDATE_SIZE(input_size);

    size_t compressed_size = compress_data(input_data, input_size, compressed_data, sizeof(compressed_data));
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, sizeof(decompressed_data));
    if (decompressed_size == 0 || decompressed_size != input_size) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Data mismatch after compression/decompression\n");
        return EXIT_FAILURE;
    }

    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f\n", (float)input_size / compressed_size);
    printf("Data verified successfully\n");

    return EXIT_SUCCESS;
}