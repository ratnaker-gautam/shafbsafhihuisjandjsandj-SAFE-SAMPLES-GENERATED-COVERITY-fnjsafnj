//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define HEADER_SIZE 4
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define WRITE_HEADER(buf, size) \
    do { \
        uint32_t header = MAGIC_NUMBER; \
        memcpy((buf), &header, sizeof(header)); \
        memcpy((buf) + sizeof(header), &(size), sizeof(size)); \
    } while(0)

#define READ_HEADER(buf, size_ptr) \
    do { \
        uint32_t magic; \
        memcpy(&magic, (buf), sizeof(magic)); \
        if (magic != MAGIC_NUMBER) { return EXIT_FAILURE; } \
        memcpy((size_ptr), (buf) + sizeof(magic), sizeof(*(size_ptr))); \
    } while(0)

static size_t compress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    size_t output_size = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        CHECK_BOUNDS(output_size, MAX_OUTPUT_SIZE);
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        output[output_size++] = compressed;
    }
    WRITE_HEADER(output, input_size);
    return output_size;
}

static size_t decompress_data(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size < HEADER_SIZE) return 0;
    size_t original_size;
    READ_HEADER(input, &original_size);
    if (original_size > MAX_INPUT_SIZE) return 0;
    size_t output_size = 0;
    for (size_t i = HEADER_SIZE; i < input_size && output_size < original_size; i++) {
        uint8_t compressed = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0 && output_size < original_size; j--) {
            output[output_size++] = (compressed >> (j * 2)) & 0x03;
        }
    }
    return output_size;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    size_t input_size;
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Input size out of bounds\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        unsigned int value;
        if (scanf("%u", &value) != 1) {
            printf("Invalid input byte\n");
            return EXIT_FAILURE;
        }
        if (value > UINT8_MAX) {
            printf("Input byte out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)value;
    }
    
    size_t compressed_size = compress_data(input_data, input_size, compressed_data);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data);
    if (decompressed_size != input_size) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression successful\n");
    printf("Original size: %zu, Compressed size: %zu\n", input_size, compressed_size);
    printf("Compression ratio: %.2f\n", (float)input_size / compressed_size);
    
    return EXIT_SUCCESS;
}