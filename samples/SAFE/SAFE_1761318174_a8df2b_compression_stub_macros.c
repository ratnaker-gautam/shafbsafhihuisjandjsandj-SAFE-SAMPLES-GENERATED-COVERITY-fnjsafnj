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

static int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (output_size < HEADER_SIZE + input_size * COMPRESSED_MULTIPLIER) {
        return 0;
    }

    CompressedHeader *header = (CompressedHeader *)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;

    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i++) {
        if (output_idx + 2 > output_size) {
            return 0;
        }
        output[output_idx++] = input[i];
        output[output_idx++] = (uint8_t)(input[i] ^ 0xFF);
    }
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input_size < HEADER_SIZE) {
        return 0;
    }

    const CompressedHeader *header = (const CompressedHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return 0;
    }

    size_t original_size = header->original_size;
    if (original_size == 0 || original_size > MAX_INPUT_SIZE) {
        return 0;
    }

    if (output_size < original_size) {
        return 0;
    }

    size_t compressed_size = HEADER_SIZE + original_size * 2;
    if (input_size < compressed_size) {
        return 0;
    }

    for (size_t i = 0; i < original_size; i++) {
        size_t compressed_idx = HEADER_SIZE + i * 2;
        uint8_t byte1 = input[compressed_idx];
        uint8_t byte2 = input[compressed_idx + 1];
        if (byte2 != (uint8_t)(byte1 ^ 0xFF)) {
            return 0;
        }
        output[i] = byte1;
    }
    return original_size;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER + HEADER_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);

    char buffer[MAX_INPUT_SIZE + 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[--input_len] = '\0';
    }

    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }

    memcpy(input_data, buffer, input_len);
    
    if (!validate_input(input_data, input_len)) {
        return EXIT_FAILURE;
    }

    size_t compressed_size = compress_data(input_data, input_len, compressed_data, sizeof(compressed_data));
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }

    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);

    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, sizeof(decompressed_data));
    if (decompressed_size == 0) {
        return EXIT_FAILURE;
    }

    if (decompressed_size != input_len || memcmp(input_data, decompressed_data, input_len) != 0) {
        return EXIT_FAILURE;
    }

    printf("Decompression successful\n");
    printf("Original data: %s\n", input_data);
    printf("Decompressed data: %s\n", decompressed_data);

    return EXIT_SUCCESS;
}