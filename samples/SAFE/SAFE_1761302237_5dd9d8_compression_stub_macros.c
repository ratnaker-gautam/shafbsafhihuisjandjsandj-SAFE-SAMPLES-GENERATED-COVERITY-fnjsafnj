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
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        size_t chunk_len = MIN(CHUNK_SIZE, len - i);
        uint8_t compressed = 0;
        for (size_t j = 0; j < chunk_len; j++) {
            compressed |= ((input[i + j] & 1) << j);
        }
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
            output[out_idx++] = (input[i] & (1 << j)) ? MASK_BYTE : 0;
        }
    }
    return out_idx;
}

static int read_input(uint8_t *buffer, size_t max_size, size_t *read_len) {
    char input_line[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    if (input_len > max_size * 2) {
        return EXIT_FAILURE;
    }
    size_t byte_count = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        char byte_str[3] = {0};
        size_t copy_len = MIN(2, input_len - i);
        memcpy(byte_str, &input_line[i], copy_len);
        char *endptr;
        unsigned long byte_val = strtoul(byte_str, &endptr, 16);
        if (endptr != byte_str + copy_len || byte_val > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        CHECK_BOUNDS(byte_count, max_size);
        buffer[byte_count++] = (uint8_t)byte_val;
    }
    *read_len = byte_count;
    return EXIT_SUCCESS;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    size_t input_len = 0;
    size_t compressed_len = 0;
    size_t decompressed_len = 0;

    printf("Enter hex data (up to %zu bytes): ", MAX_INPUT_SIZE);
    if (read_input(input_data, MAX_INPUT_SIZE, &input_len) != EXIT_SUCCESS) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (input_len == 0) {
        fprintf(stderr, "No input data\n");
        return EXIT_FAILURE;
    }

    compressed_len = compress_chunk(input_data, input_len, compressed_data);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }

    decompressed_len = decompress_chunk(compressed_data, compressed_len, decompressed_data);
    if (decompressed_len == 0) {
        fprintf(stderr, "Decompression failed\n");
        return EXIT_FAILURE;
    }

    printf("Original: ");
    print_hex(input_data, input_len);
    printf("Compressed: ");
    print_hex(compressed_data, compressed_len);
    printf("Decompressed: ");
    print_hex(decompressed_data, decompressed_len);

    if (memcmp(input_data, decompressed_data, input_len) != 0) {
        fprintf(stderr, "Verification failed\n");
        return EXIT_FAILURE;
    }

    printf("Compression successful\n");
    return EXIT_SUCCESS;
}