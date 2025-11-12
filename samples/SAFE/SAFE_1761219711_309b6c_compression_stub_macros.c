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

    size_t output_pos = 0;
    size_t i = 0;

    while (i < input_len) {
        if (output_pos + 2 > output_max) {
            return 0;
        }

        uint8_t current = input[i];
        size_t run_length = 1;

        while (i + run_length < input_len && input[i + run_length] == current && run_length < MAX_RUN_LENGTH) {
            run_length++;
        }

        if (run_length >= COMPRESSION_THRESHOLD || current == ESCAPE_CHAR) {
            if (output_pos + 3 > output_max) {
                return 0;
            }
            output[output_pos++] = ESCAPE_CHAR;
            output[output_pos++] = (uint8_t)run_length;
            output[output_pos++] = current;
            i += run_length;
        } else {
            output[output_pos++] = current;
            i++;
        }
    }

    return output_pos;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }

    size_t output_pos = 0;
    size_t i = 0;

    while (i < input_len) {
        if (output_pos >= output_max) {
            return 0;
        }

        if (input[i] == ESCAPE_CHAR) {
            if (i + 2 >= input_len) {
                return 0;
            }
            uint8_t run_length = input[i + 1];
            uint8_t value = input[i + 2];

            if (output_pos + run_length > output_max) {
                return 0;
            }

            for (uint8_t j = 0; j < run_length; j++) {
                output[output_pos++] = value;
            }
            i += 3;
        } else {
            output[output_pos++] = input[i++];
        }
    }

    return output_pos;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];

    printf("Enter input data length (1-%d): ", MAX_INPUT_SIZE);
    int input_len;
    if (scanf("%d", &input_len) != 1) {
        printf("Invalid input length\n");
        return EXIT_FAILURE;
    }
    if (input_len <= 0 || input_len > MAX_INPUT_SIZE) {
        printf("Input length out of range\n");
        return EXIT_FAILURE;
    }

    printf("Enter %d bytes as hexadecimal values (00-FF): ", input_len);
    for (int i = 0; i < input_len; i++) {
        unsigned int byte;
        if (scanf("%02X", &byte) != 1) {
            printf("Invalid byte input\n");
            return EXIT_FAILURE;
        }
        if (byte > 0xFF) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }

    size_t compressed_len = compress_data(input_data, (size_t)input_len, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_len == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (decompressed_len != (size_t)input_len || memcmp(input_data, decompressed_data, decompressed_len) != 0) {
        printf("Compression/decompression mismatch\n");
        return EXIT_FAILURE;
    }

    printf("Original data: ");
    print_hex(input_data, (size_t)input_len);
    printf("Compressed data: ");
    print_hex(compressed_data, compressed_len);
    printf("Decompressed data: ");
    print_hex(decompressed_data, decompressed_len);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_len / (double)input_len) * 100.0);

    return EXIT_SUCCESS;
}