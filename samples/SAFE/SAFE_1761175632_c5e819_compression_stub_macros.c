//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 3
#define ESCAPE_CHAR 0xFF
#define MAX_RUN_LENGTH 255

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    if (input_len > MAX_INPUT_SIZE) {
        return 0;
    }

    size_t output_idx = 0;
    size_t i = 0;

    while (i < input_len) {
        if (output_idx + 2 > output_max) {
            return 0;
        }

        uint8_t current = input[i];
        size_t run_length = 1;

        while (i + run_length < input_len && input[i + run_length] == current && run_length < MAX_RUN_LENGTH) {
            run_length++;
        }

        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (output_idx + 3 > output_max) {
                return 0;
            }
            output[output_idx++] = ESCAPE_CHAR;
            output[output_idx++] = (uint8_t)run_length;
            output[output_idx++] = current;
            i += run_length;
        } else {
            output[output_idx++] = current;
            i++;
        }
    }

    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }

    size_t output_idx = 0;
    size_t i = 0;

    while (i < input_len) {
        if (output_idx >= output_max) {
            return 0;
        }

        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_len) {
                return 0;
            }
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];

            if (output_idx + run_length > output_max) {
                return 0;
            }

            for (uint8_t j = 0; j < run_length; j++) {
                output[output_idx++] = value;
            }
            i += 3;
        } else {
            output[output_idx++] = input[i];
            i++;
        }
    }

    return output_idx;
}

static int validate_input(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];

    printf("Enter input data (up to %d bytes as hex values, space separated): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int value;
    while (scanf("%02x", &value) == 1) {
        if (input_len >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return 1;
        }
        input_data[input_len++] = (uint8_t)value;
    }

    if (input_len == 0) {
        printf("No valid input provided\n");
        return 1;
    }

    if (!validate_input(input_data, input_len)) {
        printf("Invalid input\n");
        return 1;
    }

    size_t compressed_len = compress_data(input_data, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed\n");
        return 1;
    }

    size_t decompressed_len = decompress_data(compressed, compressed_len, decompressed, MAX_INPUT_SIZE);
    if (decompressed_len == 0) {
        printf("Decompression failed\n");
        return 1;
    }

    if (decompressed_len != input_len || memcmp(input_data, decompressed, input_len) != 0) {
        printf("Round-trip verification failed\n");
        return 1;
    }

    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_len / input_len) * 100);

    printf("Compressed data: ");
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");

    return 0;
}