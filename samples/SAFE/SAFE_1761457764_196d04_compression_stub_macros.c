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
#define CHECK_BOUNDS(len, max) if ((len) > (max) || (len) < 0) { return EXIT_FAILURE; }
#define SAFE_ADD(a, b) (((b) > 0) && ((a) > INT_MAX - (b))) ? INT_MAX : ((a) + (b))

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || input == NULL || output == NULL) {
        return 0;
    }
    size_t compressed = 0;
    for (size_t i = 0; i < len; i++) {
        output[compressed++] = input[i] ^ MASK_BYTE;
        if (i % COMPRESSION_RATIO == 1) {
            output[compressed++] = (input[i-1] & 0xF0) | (input[i] & 0x0F);
        }
    }
    return compressed;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || input == NULL || output == NULL) {
        return 0;
    }
    size_t decompressed = 0;
    for (size_t i = 0; i < len; i++) {
        output[decompressed++] = input[i] ^ MASK_BYTE;
        if (i % COMPRESSION_RATIO == 1 && i + 1 < len) {
            uint8_t combined = input[++i];
            output[decompressed++] = (combined & 0xF0) | (output[decompressed-2] & 0x0F);
            output[decompressed-2] = (output[decompressed-2] & 0xF0) | (combined & 0x0F);
        }
    }
    return decompressed;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    
    printf("Enter data size (0-%d): ", MAX_INPUT_SIZE);
    int size;
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    CHECK_BOUNDS(size, MAX_INPUT_SIZE);
    
    if (size == 0) {
        printf("No data to process\n");
        return EXIT_SUCCESS;
    }
    
    printf("Enter %d bytes of data: ", size);
    for (int i = 0; i < size; i++) {
        unsigned int byte;
        if (scanf("%2x", &byte) != 1) {
            printf("Invalid byte input\n");
            return EXIT_FAILURE;
        }
        CHECK_BOUNDS(byte, UINT8_MAX);
        input[i] = (uint8_t)byte;
    }
    
    size_t comp_size = compress_chunk(input, (size_t)size, compressed);
    CHECK_BOUNDS(comp_size, MAX_OUTPUT_SIZE);
    
    size_t decomp_size = decompress_chunk(compressed, comp_size, decompressed);
    CHECK_BOUNDS(decomp_size, MAX_INPUT_SIZE);
    
    if (decomp_size != (size_t)size || memcmp(input, decompressed, decomp_size) != 0) {
        printf("Compression/decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original data: ");
    for (int i = 0; i < size; i++) {
        printf("%02X ", input[i]);
    }
    printf("\nCompressed data: ");
    for (size_t i = 0; i < comp_size; i++) {
        printf("%02X ", compressed[i]);
    }
    printf("\nDecompressed data: ");
    for (size_t i = 0; i < decomp_size; i++) {
        printf("%02X ", decompressed[i]);
    }
    printf("\nCompression ratio: %.2f\n", (float)comp_size / (float)size);
    
    return EXIT_SUCCESS;
}