//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER)

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t* input, size_t input_len, uint8_t* output) {
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        CHECK_BOUNDS(output_idx, MAX_COMPRESSED_SIZE - 2);
        uint8_t current = input[i];
        output[output_idx++] = (current >> SHIFT_AMOUNT) & BYTE_MASK;
        output[output_idx++] = current & BYTE_MASK;
    }
    return output_idx;
}

static size_t decompress_data(const uint8_t* input, size_t input_len, uint8_t* output) {
    if (input_len % 2 != 0) {
        return 0;
    }
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        CHECK_BOUNDS(output_idx, MAX_INPUT_SIZE);
        uint8_t high = input[i] & BYTE_MASK;
        uint8_t low = input[i + 1] & BYTE_MASK;
        output[output_idx++] = (high << SHIFT_AMOUNT) | low;
    }
    return output_idx;
}

static int read_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    printf("Enter input data (up to %zu bytes): ", max_size);
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    if (len > max_size) {
        return EXIT_FAILURE;
    }
    memcpy(buffer, line, len);
    *actual_size = len;
    return EXIT_SUCCESS;
}

static void print_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_COMPRESSED_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t input_len = 0;
    size_t compressed_len = 0;
    size_t decompressed_len = 0;

    if (read_input(input, MAX_INPUT_SIZE, &input_len) != EXIT_SUCCESS) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }

    compressed_len = compress_data(input, input_len, compressed);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression error\n");
        return EXIT_FAILURE;
    }

    decompressed_len = decompress_data(compressed, compressed_len, decompressed);
    if (decompressed_len != input_len || memcmp(input, decompressed, input_len) != 0) {
        fprintf(stderr, "Decompression error\n");
        return EXIT_FAILURE;
    }

    printf("Original data (%zu bytes): ", input_len);
    print_hex(input, input_len);
    printf("Compressed data (%zu bytes): ", compressed_len);
    print_hex(compressed, compressed_len);
    printf("Decompression verified successfully\n");

    return EXIT_SUCCESS;
}