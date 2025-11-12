//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
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
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    if (*output_size < HEADER_SIZE + input_size / COMPRESSION_FACTOR) {
        return -1;
    }

    CompressionHeader *header = (CompressionHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        if (out_idx >= *output_size) {
            return -1;
        }
        uint8_t compressed_byte = 0;
        for (int j = 0; j < COMPRESSION_FACTOR; j++) {
            if (i + j < input_size) {
                compressed_byte ^= input[i + j];
            }
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
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    if (*output_size < header->original_size) {
        return -1;
    }

    size_t compressed_data_size = input_size - HEADER_SIZE;
    size_t expected_compressed_size = (header->original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (compressed_data_size != expected_compressed_size) {
        return -1;
    }

    for (size_t i = 0; i < header->original_size; i++) {
        size_t compressed_idx = i / COMPRESSION_FACTOR;
        if (HEADER_SIZE + compressed_idx >= input_size) {
            return -1;
        }
        output[i] = input[HEADER_SIZE + compressed_idx];
    }

    *output_size = header->original_size;
    return 0;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);

    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_size >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return EXIT_FAILURE;
        }
        input_buffer[input_size++] = (uint8_t)c;
    }

    if (input_size == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }

    size_t compressed_size = sizeof(compressed_buffer);
    if (compress_data(input_buffer, input_size, compressed_buffer, &compressed_size) != 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);

    size_t decompressed_size = sizeof(decompressed_buffer);
    if (decompress_data(compressed_buffer, compressed_size, decompressed_buffer, &decompressed_size) != 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (decompressed_size != input_size || memcmp(input_buffer, decompressed_buffer, input_size) != 0) {
        printf("Decompression verification failed\n");
        return EXIT_FAILURE;
    }

    printf("Decompression successful: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        putchar(decompressed_buffer[i]);
    }
    putchar('\n');

    return EXIT_SUCCESS;
}