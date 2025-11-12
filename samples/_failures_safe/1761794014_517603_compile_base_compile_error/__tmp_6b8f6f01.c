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
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }

static int validate_input(const uint8_t *data, size_t len) {
    if (data == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!validate_input(input, input_len)) return 0;
    if (output == NULL || output_max < HEADER_SIZE + (input_len + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO) return 0;
    
    uint32_t *header = (uint32_t*)output;
    header[0] = MAGIC_NUMBER;
    header[1] = (uint32_t)input_len;
    
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_RATIO) {
        if (output_idx >= output_max) return 0;
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < input_len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        output[output_idx++] = compressed;
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len < HEADER_SIZE) return 0;
    
    uint32_t *header = (uint32_t*)input;
    if (header[0] != MAGIC_NUMBER) return 0;
    uint32_t original_size = header[1];
    if (original_size == 0 || original_size > MAX_INPUT_SIZE) return 0;
    if (output_max < original_size) return 0;
    
    size_t input_idx = HEADER_SIZE;
    size_t output_idx = 0;
    
    while (output_idx < original_size && input_idx < input_len) {
        uint8_t compressed = input[input_idx++];
        for (int shift = 6; shift >= 0 && output_idx < original_size; shift -= 2) {
            output[output_idx++] = (compressed >> shift) & 0x03;
        }
    }
    
    return output_idx;
}

static void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size;
    if (scanf("%d", &size) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (size <= 0 || size > MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (hex values 00-FF): ", size);
    for (int i = 0; i < size; i++) {
        unsigned int byte;
        if (scanf("%02X", &byte) != 1) {
            printf("Invalid byte\n");
            return EXIT_FAILURE;
        }
        if (byte > 0xFF) {
            printf("Byte out of range\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte;
    }
    
    printf("\nOriginal data (%d bytes):\n", size);
    print_hex(input_data, size);
    
    size_t compressed_size = compress_data(input_data, size, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed_data, compressed_size);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size != (size_t)size) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("\n