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
            output[output_idx++] = input[i++];
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

    printf("Enter input data (up to %d bytes as hex digits, no spaces): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }

    if (hex_len % 2 != 0) {
        fprintf(stderr, "Invalid hex input: must have even number of digits\n");
        return EXIT_FAILURE;
    }

    size_t input_len = hex_len / 2;
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char *endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > UINT8_MAX) {
            fprintf(stderr, "Invalid hex digit\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }

    if (!validate_input(input_data, input_len)) {
        fprintf(stderr, "Invalid input data\n");
        return EXIT_FAILURE;
    }

    size_t compressed_len = compress_data(input_data, input_len, compressed, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed_len);
    for (size_t i = 0; i < compressed_len; i++) {
        printf("%02X", compressed[i]);
    }
    printf("\n");

    size_t decompressed_len = decompress_data(compressed, compressed_len, decompressed, MAX_INPUT_SIZE);
    if (decompressed_len == 0