//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

uint32_t calculate_checksum(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum + data[i]) & 0xFFFFFFFF;
    }
    return sum;
}

int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) return 0;
    if (*output_len < HEADER_SIZE + input_len / COMPRESSION_FACTOR) return 0;

    uint32_t header = MAGIC_NUMBER;
    memcpy(output, &header, sizeof(header));
    size_t out_idx = HEADER_SIZE;

    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        CHECK_BOUNDS(out_idx, *output_len);
        output[out_idx++] = compressed;
    }

    *output_len = out_idx;
    return 1;
}

int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len < HEADER_SIZE) return 0;
    uint32_t header;
    memcpy(&header, input, sizeof(header));
    if (header != MAGIC_NUMBER) return 0;

    size_t comp_len = input_len - HEADER_SIZE;
    size_t required_out = comp_len * COMPRESSION_FACTOR;
    if (required_out > *output_len || required_out > MAX_INPUT_SIZE) return 0;

    size_t out_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_len; i++) {
        uint8_t compressed = input[i];
        for (int j = COMPRESSION_FACTOR - 1; j >= 0; j--) {
            uint8_t decompressed = (compressed >> (j * 2)) & 0x03;
            CHECK_BOUNDS(out_idx, *output_len);
            output[out_idx++] = decompressed;
        }
    }

    *output_len = out_idx;
    return 1;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];

    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    int input_size;
    if (scanf("%d", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);

    printf("Enter %d bytes (0-255): ", input_size);
    for (int i = 0; i < input_size; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1 || byte_val < 0 || byte_val > 255) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        input[i] = (uint8_t)byte_val;
    }

    size_t comp_len = sizeof(compressed);
    if (!compress_data(input, input_size, compressed, &comp_len)) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", comp_len);
    for (size_t i = 0; i < comp_len; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\n");

    uint32_t orig_checksum = calculate_checksum(input, input_size);
    printf("Original checksum: %08X\n", orig_checksum);

    size_t decomp_len = sizeof(decompressed);
    if (!decompress_data(compressed, comp_len, decompressed, &decomp_len)) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }

    if (decomp_len != input_size) {
        printf("Decompression size mismatch\n");
        return EXIT_FAILURE;
    }

    uint32_t decomp_checksum = calculate_checksum(decompressed, decomp_len);
    printf("Decompressed checksum: %08X\n", decomp_checksum);

    if (orig_checksum != decomp_checksum) {
        printf("Checksum mismatch\n");
        return EXIT_FAILURE;
    }

    printf("Compression and decompression successful\n");
    return EXIT_SUCCESS;
}