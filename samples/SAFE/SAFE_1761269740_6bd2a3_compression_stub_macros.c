//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return -1;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return -1;
    }

    size_t out_idx = 0;
    size_t in_idx = 0;

    while (in_idx < input_len) {
        if (out_idx + COMPRESSED_CHUNK > MAX_OUTPUT_SIZE) {
            return -1;
        }

        uint8_t current = input[in_idx];
        uint8_t count = 1;

        while (in_idx + count < input_len && count < UINT8_MAX && input[in_idx + count] == current) {
            count++;
        }

        output[out_idx++] = count;
        output[out_idx++] = current;
        in_idx += count;
    }

    *output_len = out_idx;
    return 0;
}

static int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return -1;
    }
    if (input_len == 0 || input_len > MAX_OUTPUT_SIZE) {
        return -1;
    }

    size_t out_idx = 0;
    size_t in_idx = 0;

    while (in_idx < input_len) {
        if (in_idx + 1 >= input_len) {
            return -1;
        }

        uint8_t count = input[in_idx++];
        uint8_t value = input[in_idx++];

        if (out_idx + count > MAX_INPUT_SIZE) {
            return -1;
        }

        for (uint8_t i = 0; i < count; i++) {
            output[out_idx++] = value;
        }
    }

    *output_len = out_idx;
    return 0;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t input_len = 0;
    size_t compressed_len = 0;
    size_t decompressed_len = 0;

    printf("Enter input data (hex string, max %d bytes): ", MAX_INPUT_SIZE);
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

    for (size_t i = 0; i < hex_len; i += 2) {
        char hex_byte[3] = {hex_input[i], hex_input[i + 1], '\0'};
        char *endptr;
        long byte_val = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || byte_val < 0 || byte_val > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_data[input_len++] = (uint8_t)byte_val;
    }

    if (compress_data(input_data, input_len, compressed, &compressed_len) != 0) {
        return EXIT_FAILURE;
    }

    printf("Compressed data: ");
    print_hex(compressed, compressed_len);

    if (decompress_data(compressed, compressed_len, decompressed, &decompressed_len) != 0) {
        return EXIT_FAILURE;
    }

    if (decompressed_len != input_len || memcmp(input_data, decompressed, input_len) != 0) {
        return EXIT_FAILURE;
    }

    printf("Decompressed data: ");
    print_hex(decompressed, decompressed_len);

    return EXIT_SUCCESS;
}