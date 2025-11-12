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
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size % CHUNK_SIZE != 0) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            compressed_byte |= ((input[i + j] & 1) << j);
        }
        output[output_size++] = compressed_byte ^ MASK_BYTE;
    }
    return output_size;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i++) {
        uint8_t compressed_byte = input[i] ^ MASK_BYTE;
        for (size_t j = 0; j < CHUNK_SIZE; j++) {
            output[output_size++] = (compressed_byte & (1 << j)) ? 0xFF : 0x00;
        }
    }
    return output_size;
}

static int compare_data(const uint8_t *a, const uint8_t *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    size_t input_size = 0;
    int ch;
    while ((ch = getchar()) != EOF && input_size < MAX_INPUT_SIZE) {
        input_data[input_size++] = (uint8_t)ch;
    }
    if (input_size == 0) {
        return EXIT_FAILURE;
    }
    if (input_size % CHUNK_SIZE != 0) {
        size_t padding = CHUNK_SIZE - (input_size % CHUNK_SIZE);
        if (input_size + padding > MAX_INPUT_SIZE) {
            return EXIT_FAILURE;
        }
        for (size_t i = 0; i < padding; i++) {
            input_data[input_size++] = 0;
        }
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data);
    if (compressed_size == 0) {
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size != input_size) {
        return EXIT_FAILURE;
    }
    
    if (!compare_data(input_data, decompressed_data, input_size)) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < compressed_size; i++) {
        if (putchar(compressed_data[i]) == EOF) {
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}