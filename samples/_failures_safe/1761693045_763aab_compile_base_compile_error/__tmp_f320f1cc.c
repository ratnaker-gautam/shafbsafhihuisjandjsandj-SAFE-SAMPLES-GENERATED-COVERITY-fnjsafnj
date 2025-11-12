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
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressHeader;

int compress_data(const uint8_t* input, size_t input_size, uint8_t** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }

    size_t compressed_max = HEADER_SIZE + (input_size * COMPRESSED_MULTIPLIER);
    if (compressed_max > SIZE_MAX) {
        return EXIT_FAILURE;
    }

    uint8_t* compressed = malloc(compressed_max);
    CHECK_NULL(compressed);

    CompressHeader* header = (CompressHeader*)compressed;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t compressed_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i++) {
        CHECK_BOUNDS(compressed_idx, compressed_max);
        compressed[compressed_idx++] = input[i];
        if (i + 1 < input_size && input[i] == input[i + 1]) {
            uint8_t count = 1;
            while (i + 1 < input_size && input[i] == input[i + 1] && count < UINT8_MAX) {
                count++;
                i++;
            }
            CHECK_BOUNDS(compressed_idx, compressed_max);
            compressed[compressed_idx++] = count;
        }
    }

    *output = compressed;
    *output_size = compressed_idx;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t** output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }

    const CompressHeader* header = (const CompressHeader*)input;
    if (header->magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }

    uint8_t* decompressed = malloc(header->original_size);
    CHECK_NULL(decompressed);

    size_t input_idx = HEADER_SIZE;
    size_t output_idx = 0;
    while (input_idx < input_size && output_idx < header->original_size) {
        CHECK_BOUNDS(input_idx, input_size);
        uint8_t current = input[input_idx++];
        CHECK_BOUNDS(output_idx, header->original_size);
        decompressed[output_idx++] = current;

        if (input_idx < input_size && output_idx < header->original_size) {
            uint8_t next = input[input_idx];
            if (next > 1 && next <= UINT8_MAX) {
                for (uint8_t j = 1; j < next && output_idx < header->original_size; j++) {
                    CHECK_BOUNDS(output_idx, header->original_size);
                    decompressed[output_idx++] = current;
                }
                input_idx++;
            }
        }
    }

    if (output_idx != header->original_size) {
        free(decompressed);
        return EXIT_FAILURE;
    }

    *output = decompressed;
    *output_size = header->original_size;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;

    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
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
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid data\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > UINT8_MAX) {
            printf("Invalid data\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }

    uint8_t* compressed = NULL;
    size_t compressed_size = 0;
    if (compress_data(input_data, input_size, &compressed, &compressed_size) != EXIT_SUCCESS) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu