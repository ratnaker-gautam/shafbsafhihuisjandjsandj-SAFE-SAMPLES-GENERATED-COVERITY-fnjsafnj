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
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t stub_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return 0;
    }
    if (output_size < input_size * COMPRESSION_RATIO) {
        return 0;
    }

    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_end = i + CHUNK_SIZE;
        if (chunk_end > input_size) {
            chunk_end = input_size;
        }
        for (size_t j = i; j < chunk_end; j++) {
            CHECK_BOUNDS(output_idx, output_size);
            output[output_idx++] = input[j] ^ MASK_BYTE;
        }
        if (output_idx < output_size) {
            output[output_idx++] = (uint8_t)(chunk_end - i);
        }
    }
    return output_idx;
}

static size_t stub_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL) {
        return 0;
    }
    if (input_size == 0 || input_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    if (output_size < input_size) {
        return 0;
    }

    size_t output_idx = 0;
    size_t input_idx = 0;
    while (input_idx < input_size) {
        uint8_t chunk_size = input[input_idx++];
        if (chunk_size == 0 || chunk_size > CHUNK_SIZE) {
            return 0;
        }
        for (uint8_t i = 0; i < chunk_size; i++) {
            CHECK_BOUNDS(input_idx, input_size);
            CHECK_BOUNDS(output_idx, output_size);
            output[output_idx++] = input[input_idx++] ^ MASK_BYTE;
        }
    }
    return output_idx;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    char input_buffer[MAX_INPUT_SIZE * 2];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    if (input_len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    memcpy(input_data, input_buffer, input_len);

    size_t compressed_size = stub_compress(input_data, input_len, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }

    size_t decompressed_size = stub_decompress(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0) {
        return EXIT_FAILURE;
    }

    if (decompressed_size != input_len || memcmp(input_data, decompressed_data, input_len) != 0) {
        return EXIT_FAILURE;
    }

    printf("Original size: %zu\n", input_len);
    printf("Compressed size: %zu\n", compressed_size);
    printf("Decompression verified successfully.\n");

    return EXIT_SUCCESS;
}