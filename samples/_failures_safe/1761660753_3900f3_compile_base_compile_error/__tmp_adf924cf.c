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
} CompressHeader;

static size_t safe_add(size_t a, size_t b) {
    if (a > SIZE_MAX - b) {
        return SIZE_MAX;
    }
    return a + b;
}

static int compress_data(const uint8_t *input, size_t input_size, uint8_t **output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }

    size_t compressed_buffer_size = safe_add(input_size, HEADER_SIZE);
    if (compressed_buffer_size == SIZE_MAX) {
        return EXIT_FAILURE;
    }
    compressed_buffer_size = safe_add(compressed_buffer_size, COMPRESSED_MULTIPLIER * input_size);
    if (compressed_buffer_size == SIZE_MAX) {
        return EXIT_FAILURE;
    }

    uint8_t *compressed = malloc(compressed_buffer_size);
    CHECK_NULL(compressed);

    CompressHeader *header = (CompressHeader *)compressed;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    uint8_t *data_start = compressed + HEADER_SIZE;
    for (size_t i = 0; i < input_size; i++) {
        CHECK_BOUNDS(i, input_size);
        data_start[i * 2] = input[i];
        data_start[i * 2 + 1] = input[i] ^ 0xFF;
    }

    *output = compressed;
    *output_size = HEADER_SIZE + input_size * 2;
    return EXIT_SUCCESS;
}

static int decompress_data(const uint8_t *input, size_t input_size, uint8_t **output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }

    const CompressHeader *header = (const CompressHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    size_t expected_size = HEADER_SIZE + header->original_size * 2;
    if (input_size != expected_size) {
        return EXIT_FAILURE;
    }

    uint8_t *decompressed = malloc(header->original_size);
    CHECK_NULL(decompressed);

    const uint8_t *data_start = input + HEADER_SIZE;
    for (size_t i = 0; i < header->original_size; i++) {
        CHECK_BOUNDS(i, header->original_size);
        uint8_t byte1 = data_start[i * 2];
        uint8_t byte2 = data_start[i * 2 + 1];
        if ((byte1 ^ 0xFF) != byte2) {
            free(decompressed);
            return EXIT_FAILURE;
        }
        decompressed[i] = byte1;
    }

    *output = decompressed;
    *output_size = header->original_size;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;

    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }

    printf("Enter %zu bytes of data: ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int byte;
        if (scanf("%2x", &byte) != 1) {
            printf("Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte < 0 || byte > 0xFF) {
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

    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02X", compressed[i]);
    }
    printf("\n");

    uint8_t *