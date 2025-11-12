//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAGIC_HEADER 0xDEADBEEF
#define MAX_CHUNKS 128

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    uint32_t header;
    uint16_t chunk_size;
    uint8_t data[COMPRESSED_CHUNK];
} compressed_chunk_t;

static int compress_data(const uint8_t *input, size_t input_len, compressed_chunk_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return -1;
    }
    if (input_len > MAX_INPUT_SIZE) {
        return -1;
    }

    size_t num_chunks = (input_len + COMPRESSED_CHUNK - 1) / COMPRESSED_CHUNK;
    if (num_chunks > MAX_CHUNKS) {
        return -1;
    }

    for (size_t i = 0; i < num_chunks; i++) {
        output[i].header = MAGIC_HEADER;
        size_t chunk_start = i * COMPRESSED_CHUNK;
        size_t remaining = input_len - chunk_start;
        size_t current_chunk_size = MIN(COMPRESSED_CHUNK, remaining);
        output[i].chunk_size = (uint16_t)current_chunk_size;

        for (size_t j = 0; j < current_chunk_size; j++) {
            output[i].data[j] = input[chunk_start + j] ^ 0xAA;
        }
        for (size_t j = current_chunk_size; j < COMPRESSED_CHUNK; j++) {
            output[i].data[j] = 0;
        }
    }

    *output_len = num_chunks;
    return 0;
}

static int decompress_data(const compressed_chunk_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return -1;
    }

    size_t total_size = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (input[i].header != MAGIC_HEADER) {
            return -1;
        }
        if (total_size + input[i].chunk_size > MAX_INPUT_SIZE) {
            return -1;
        }
        total_size += input[i].chunk_size;
    }

    if (total_size > *output_len) {
        return -1;
    }

    size_t offset = 0;
    for (size_t i = 0; i < input_len; i++) {
        for (size_t j = 0; j < input[i].chunk_size; j++) {
            output[offset++] = input[i].data[j] ^ 0xAA;
        }
    }

    *output_len = total_size;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_chunk_t compressed[MAX_CHUNKS];
    size_t compressed_len = MAX_CHUNKS;
    size_t decompressed_len = MAX_INPUT_SIZE;

    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char *)input_data, sizeof(input_data), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    size_t input_len = strlen((char *)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }

    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }

    if (compress_data(input_data, input_len, compressed, &compressed_len) != 0) {
        printf("Compression failed.\n");
        return EXIT_FAILURE;
    }

    printf("Compressed into %zu chunks.\n", compressed_len);

    if (decompress_data(compressed, compressed_len, output_data, &decompressed_len) != 0) {
        printf("Decompression failed.\n");
        return EXIT_FAILURE;
    }

    if (input_len != decompressed_len || memcmp(input_data, output_data, input_len) != 0) {
        printf("Verification failed.\n");
        return EXIT_FAILURE;
    }

    printf("Decompressed successfully: %s\n", output_data);
    return EXIT_SUCCESS;
}