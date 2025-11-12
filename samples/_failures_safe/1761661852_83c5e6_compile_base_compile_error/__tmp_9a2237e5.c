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
#define MAGIC_HEADER 0xABCD1234

#define CHECK_BOUNDS(ptr, size, max) do { \
    if ((size) > (max) || (ptr) == NULL) { \
        return EXIT_FAILURE; \
    } \
} while(0)

#define VALIDATE_INPUT(input, len) do { \
    if ((input) == NULL || (len) == 0 || (len) > MAX_INPUT_SIZE) { \
        return EXIT_FAILURE; \
    } \
} while(0)

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0 || input_len > CHUNK_SIZE) {
        return 0;
    }
    size_t compressed = 0;
    for (size_t i = 0; i < input_len; i += COMPRESSION_RATIO) {
        if (compressed >= MAX_OUTPUT_SIZE) {
            return 0;
        }
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < input_len; j++) {
            compressed_byte = (compressed_byte << 2) | (input[i + j] >> 6);
        }
        output[compressed++] = compressed_byte;
    }
    *output_len = compressed;
    return 1;
}

static int decompress_chunk(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0 || input_len > CHUNK_SIZE) {
        return 0;
    }
    size_t decompressed = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (decompressed >= MAX_OUTPUT_SIZE) {
            return 0;
        }
        uint8_t current = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            if (decompressed < MAX_OUTPUT_SIZE) {
                output[decompressed++] = (current & 0x03) << 6;
                current >>= 2;
            }
        }
    }
    *output_len = decompressed;
    return 1;
}

static int perform_compression(const uint8_t *input, size_t input_len, compressed_data_t *result) {
    VALIDATE_INPUT(input, input_len);
    if (result == NULL) {
        return 0;
    }
    result->header = MAGIC_HEADER;
    result->original_size = (uint32_t)input_len;
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = (input_len - i) < CHUNK_SIZE ? (input_len - i) : CHUNK_SIZE;
        size_t compressed_len = 0;
        if (!compress_chunk(input + i, chunk_len, result->data + total_compressed, &compressed_len)) {
            return 0;
        }
        total_compressed += compressed_len;
        if (total_compressed > MAX_OUTPUT_SIZE) {
            return 0;
        }
    }
    result->compressed_size = (uint32_t)total_compressed;
    return 1;
}

static int perform_decompression(const compressed_data_t *compressed, uint8_t *output, size_t *output_len) {
    if (compressed == NULL || output == NULL || output_len == NULL) {
        return 0;
    }
    if (compressed->header != MAGIC_HEADER) {
        return 0;
    }
    if (compressed->original_size > MAX_INPUT_SIZE || compressed->compressed_size > MAX_OUTPUT_SIZE) {
        return 0;
    }
    size_t total_decompressed = 0;
    for (size_t i = 0; i < compressed->compressed_size; i += CHUNK_SIZE) {
        size_t chunk_len = (compressed->compressed_size - i) < CHUNK_SIZE ? (compressed->compressed_size - i) : CHUNK_SIZE;
        size_t decompressed_len = 0;
        if (!decompress_chunk(compressed->data + i, chunk_len, output + total_decompressed, &decompressed_len)) {
            return 0;
        }
        total_decompressed += decompressed_len;
        if (total_decompressed > MAX_INPUT_SIZE) {
            return 0;
        }
    }
    if (total_decompressed != compressed->original_size) {
        return 0;
    }
    *output_len = total_decompressed;
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    memset(&compressed, 0, sizeof(comp