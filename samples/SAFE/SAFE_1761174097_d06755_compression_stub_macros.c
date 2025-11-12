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
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }
#define CHECK_OVERFLOW(a, b) if ((a) > SIZE_MAX - (b)) { return EXIT_FAILURE; }

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

static int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return EXIT_FAILURE;
    }
    CHECK_OVERFLOW(input_len, 0);
    size_t required_size = HEADER_SIZE + (input_len + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (required_size > *output_len) {
        return EXIT_FAILURE;
    }
    uint32_t *header = (uint32_t *)output;
    *header = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && (i + j) < input_len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        output[out_idx++] = compressed;
    }
    *output_len = out_idx;
    return EXIT_SUCCESS;
}

static int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) {
        return EXIT_FAILURE;
    }
    if (input_len < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    uint32_t *header = (uint32_t *)input;
    if (header[0] != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    size_t orig_len = header[1];
    if (orig_len > *output_len) {
        return EXIT_FAILURE;
    }
    size_t comp_len = (orig_len + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (input_len != HEADER_SIZE + comp_len) {
        return EXIT_FAILURE;
    }
    size_t out_idx = 0;
    for (size_t i = HEADER_SIZE; i < input_len && out_idx < orig_len; i++) {
        uint8_t compressed = input[i];
        for (int j = COMPRESSION_FACTOR - 1; j >= 0 && out_idx < orig_len; j--) {
            output[out_idx++] = (compressed >> (j * 2)) & 0x03;
        }
    }
    *output_len = orig_len;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets((char *)input_data, sizeof(input_data), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen((char *)input_data, sizeof(input_data));
    if (input_len == 0) {
        return EXIT_FAILURE;
    }
    
    size_t comp_len = sizeof(compressed_data);
    if (compress_data(input_data, input_len, compressed_data, &comp_len) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_len, comp_len);
    
    size_t decomp_len = sizeof(decompressed_data);
    if (decompress_data(compressed_data, comp_len, decompressed_data, &decomp_len) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    if (decomp_len != input_len || memcmp(input_data, decompressed_data, input_len) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful. Original data: ");
    for (size_t i = 0; i < input_len; i++) {
        printf("%c", input_data[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}