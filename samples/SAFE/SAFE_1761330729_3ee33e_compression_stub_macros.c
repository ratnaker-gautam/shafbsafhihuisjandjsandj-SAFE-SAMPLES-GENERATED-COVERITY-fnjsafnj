//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 3

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_len = (i + CHUNK_SIZE <= input_len) ? CHUNK_SIZE : (input_len - i);
        for (size_t j = 0; j < chunk_len; j++) {
            chunk[j] = input[i + j];
        }
        for (size_t j = 0; j < chunk_len; j++) {
            uint8_t compressed_byte = (chunk[j] >> SHIFT_AMOUNT) & BYTE_MASK;
            if (output_index >= MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
                return output_index;
            }
            output[output_index++] = compressed_byte;
        }
    }
    return output_index;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    size_t output_index = 0;
    for (size_t i = 0; i < input_len; i++) {
        uint8_t decompressed_byte = (input[i] << SHIFT_AMOUNT) & BYTE_MASK;
        if (output_index >= MAX_INPUT_SIZE) {
            return output_index;
        }
        output[output_index++] = decompressed_byte;
    }
    return output_index;
}

static int validate_input(const char *input, size_t *length) {
    if (input == NULL || length == NULL) {
        return 0;
    }
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    *length = len;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    if (input_len == 0) {
        fprintf(stderr, "Input is empty\n");
        return EXIT_FAILURE;
    }
    uint8_t compressed[MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER] = {0};
    size_t compressed_len = compress_data((uint8_t *)input_buffer, input_len, compressed);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02x", compressed[i]);
    }
    printf("\n");
    uint8_t decompressed[MAX_INPUT_SIZE] = {0};
    size_t decompressed_len = decompress_data(compressed, compressed_len, decompressed);
    if (decompressed_len == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }
    printf("Decompressed text: ");
    for (size_t i = 0; i < decompressed_len; i++) {
        printf("%c", decompressed[i]);
    }
    printf("\n");
    if (memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    printf("Compression and decompression successful\n");
    return EXIT_SUCCESS;
}