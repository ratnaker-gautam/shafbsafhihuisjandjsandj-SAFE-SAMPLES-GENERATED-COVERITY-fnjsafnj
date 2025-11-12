//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MARKER 0xFF
#define MAX_RUN_LENGTH 255

static size_t read_input(char *buffer, size_t max_size) {
    if (fgets(buffer, max_size, stdin) == NULL) {
        return 0;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    return len;
}

static int validate_input(const char *input, size_t len) {
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 0 || input[i] > 127) {
            return 0;
        }
    }
    return 1;
}

static size_t compress_rle(const char *input, size_t input_len, uint8_t *output) {
    if (input_len == 0 || output == NULL) {
        return 0;
    }
    size_t output_index = 0;
    size_t i = 0;
    while (i < input_len) {
        char current = input[i];
        size_t run_length = 1;
        while (i + run_length < input_len && input[i + run_length] == current && run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        if (run_length > 3 || current == COMPRESSED_MARKER) {
            if (output_index + 3 >= MAX_INPUT_SIZE * 2) {
                return 0;
            }
            output[output_index++] = COMPRESSED_MARKER;
            output[output_index++] = (uint8_t)run_length;
            output[output_index++] = (uint8_t)current;
        } else {
            if (output_index + run_length >= MAX_INPUT_SIZE * 2) {
                return 0;
            }
            for (size_t j = 0; j < run_length; j++) {
                output[output_index++] = (uint8_t)current;
            }
        }
        i += run_length;
    }
    return output_index;
}

static size_t decompress_rle(const uint8_t *input, size_t input_len, char *output) {
    if (input_len == 0 || output == NULL) {
        return 0;
    }
    size_t output_index = 0;
    size_t i = 0;
    while (i < input_len) {
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_len) {
                return 0;
            }
            uint8_t run_length = input[i + 1];
            char value = (char)input[i + 2];
            if (output_index + run_length >= MAX_INPUT_SIZE) {
                return 0;
            }
            for (uint8_t j = 0; j < run_length; j++) {
                output[output_index++] = value;
            }
            i += 3;
        } else {
            if (output_index >= MAX_INPUT_SIZE) {
                return 0;
            }
            output[output_index++] = (char)input[i];
            i++;
        }
    }
    output[output_index] = '\0';
    return output_index;
}

static void print_compressed(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    uint8_t compressed[MAX_INPUT_SIZE * 2];
    char decompressed[MAX_INPUT_SIZE + 1];

    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    size_t input_len = read_input(input_buffer, sizeof(input_buffer));
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    size_t compressed_len = compress_rle(input_buffer, input_len, compressed);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed_len);
    print_compressed(compressed, compressed_len);

    size_t decompressed_len = decompress_rle(compressed, compressed_len, decompressed);
    if (decompressed_len == 0 || decompressed_len != input_len) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }

    printf("Decompressed text: %s\n", decompressed);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_len / input_len) * 100.0);

    return EXIT_SUCCESS;
}