//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || input == NULL || output == NULL) return -1;
    size_t i, j = 0;
    for (i = 0; i < len; i += COMPRESSION_RATIO) {
        uint8_t val = 0;
        size_t end = MIN(i + COMPRESSION_RATIO, len);
        for (size_t k = i; k < end; k++) {
            val = (val << 2) | (input[k] & 0x03);
        }
        if (j >= MAX_OUTPUT_SIZE) return -1;
        output[j++] = val;
    }
    return j;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t out_len) {
    if (len == 0 || input == NULL || output == NULL) return -1;
    size_t i, j = 0;
    for (i = 0; i < len; i++) {
        uint8_t val = input[i];
        for (int shift = 6; shift >= 0 && j < out_len; shift -= 2) {
            if (j >= out_len) return -1;
            output[j++] = (val >> shift) & 0x03;
        }
    }
    return j;
}

static int perform_compression(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return -1;
    if (input_len > MAX_INPUT_SIZE) return -1;
    compressed_data_t *comp = (compressed_data_t *)output;
    comp->header = MAGIC_HEADER;
    comp->original_size = (uint16_t)input_len;
    uint8_t *data_start = comp->data;
    int compressed_size = compress_chunk(input, input_len, data_start);
    if (compressed_size < 0) return -1;
    comp->compressed_size = (uint16_t)compressed_size;
    *output_len = sizeof(compressed_data_t) + compressed_size;
    return 0;
}

static int perform_decompression(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return -1;
    if (input_len < sizeof(compressed_data_t)) return -1;
    const compressed_data_t *comp = (const compressed_data_t *)input;
    if (comp->header != MAGIC_HEADER) return -1;
    if (comp->original_size > MAX_INPUT_SIZE) return -1;
    if (comp->compressed_size > MAX_OUTPUT_SIZE) return -1;
    if (input_len != sizeof(compressed_data_t) + comp->compressed_size) return -1;
    int decompressed_size = decompress_chunk(comp->data, comp->compressed_size, output, comp->original_size);
    if (decompressed_size != comp->original_size) return -1;
    *output_len = decompressed_size;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t input_len, compressed_len, decompressed_len;

    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_len) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Input size out of bounds\n");
        return EXIT_FAILURE;
    }

    printf("Enter %zu bytes (0-255): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        int val;
        if (scanf("%d", &val) != 1) {
            printf("Invalid byte value\n");
            return EXIT_FAILURE;
        }
        if (val < 0 || val > 255) {
            printf("Byte value out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)val;
    }

    if (perform_compression(input_data, input_len, compressed, &compressed_len) != 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data