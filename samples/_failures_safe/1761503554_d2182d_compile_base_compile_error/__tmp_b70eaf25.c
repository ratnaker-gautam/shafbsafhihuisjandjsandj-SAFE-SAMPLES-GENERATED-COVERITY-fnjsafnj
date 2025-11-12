//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressedHeader;

int compress_data(const uint8_t *input, size_t input_size, uint8_t **output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }

    size_t compressed_buffer_size = input_size * COMPRESSED_MULTIPLIER + HEADER_SIZE;
    if (compressed_buffer_size > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER + HEADER_SIZE) {
        return EXIT_FAILURE;
    }

    uint8_t *compressed_data = malloc(compressed_buffer_size);
    CHECK_NULL(compressed_data);

    CompressedHeader *header = (CompressedHeader *)compressed_data;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i++) {
        CHECK_BOUNDS(output_idx, compressed_buffer_size);
        compressed_data[output_idx++] = input[i];
        if (i + 1 < input_size && input[i] == input[i + 1]) {
            uint8_t count = 1;
            while (i + count < input_size && input[i] == input[i + count] && count < UINT8_MAX) {
                count++;
            }
            CHECK_BOUNDS(output_idx, compressed_buffer_size);
            compressed_data[output_idx++] = count;
            i += count - 1;
        } else {
            CHECK_BOUNDS(output_idx, compressed_buffer_size);
            compressed_data[output_idx++] = 1;
        }
    }

    *output = compressed_data;
    *output_size = output_idx;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t *input, size_t input_size, uint8_t **output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }

    const CompressedHeader *header = (const CompressedHeader *)input;
    if (header->magic != MAGIC_NUMBER || header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }

    uint8_t *decompressed_data = malloc(header->original_size);
    CHECK_NULL(decompressed_data);

    size_t input_idx = HEADER_SIZE;
    size_t output_idx = 0;
    while (input_idx < input_size && output_idx < header->original_size) {
        CHECK_BOUNDS(input_idx, input_size);
        uint8_t value = input[input_idx++];
        CHECK_BOUNDS(input_idx, input_size);
        uint8_t count = input[input_idx++];

        if (count == 0 || output_idx + count > header->original_size) {
            free(decompressed_data);
            return EXIT_FAILURE;
        }

        for (uint8_t i = 0; i < count; i++) {
            CHECK_BOUNDS(output_idx, header->original_size);
            decompressed_data[output_idx++] = value;
        }
    }

    if (output_idx != header->original_size) {
        free(decompressed_data);
        return EXIT_FAILURE;
    }

    *output = decompressed_data;
    *output_size = header->original_size;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;

    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1 || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }

    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int byte;
        if (scanf("%d", &byte) != 1 || byte < 0 || byte > 255) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }

    uint8_t *compressed = NULL;
    size_t compressed_size = 0;
    if (compress_data(input_data, input_size, &compressed, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed size: %zu\n", compressed_size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%d ", compressed[i]);
    }
    printf("\n");

    uint8_t *decompressed =