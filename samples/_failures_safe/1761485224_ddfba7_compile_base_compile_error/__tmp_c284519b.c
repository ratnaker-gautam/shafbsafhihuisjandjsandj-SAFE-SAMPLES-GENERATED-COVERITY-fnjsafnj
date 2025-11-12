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

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define VALIDATE_SIZE(sz) if ((sz) == 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < len; j++) {
            compressed = (compressed << 2) | (input[i + j] & 0x03);
        }
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t compressed = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            uint8_t value = (compressed >> (j * 2)) & 0x03;
            CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
            output[out_idx++] = value;
        }
    }
    return out_idx;
}

static int perform_compression(const uint8_t *input, size_t input_size, compressed_data_t *compressed) {
    VALIDATE_SIZE(input_size);
    CHECK_NULL(input);
    CHECK_NULL(compressed);
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint32_t)input_size;
    
    size_t compressed_size = compress_chunk(input, input_size, compressed->data);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

static int perform_decompression(const compressed_data_t *compressed, uint8_t *output, size_t *output_size) {
    CHECK_NULL(compressed);
    CHECK_NULL(output);
    CHECK_NULL(output_size);
    
    if (compressed->header != MAGIC_HEADER) {
        return EXIT_FAILURE;
    }
    
    VALIDATE_SIZE(compressed->original_size);
    
    *output_size = decompress_chunk(compressed->data, 
                                   (compressed->original_size + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO,
                                   output);
    
    if (*output_size != compressed->original_size) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    size_t output_size;
    
    printf("Enter data size (1-%d): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1) {
        return EXIT_FAILURE;
    }
    
    if (size_input <= 0 || size_input > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    printf("Enter %d bytes (0-255): ", size_input);
    for (int i = 0; i < size_input; i++) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > 255) {
            return EXIT_FAILURE;
        }
        input_data[i] = (uint8_t)byte_val;
    }
    
    if (perform_compression(input_data, size_input, &compressed) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Compression successful. Header: 0x%X, Original size: %u\n", 
           compressed.header, compressed.original_size);
    
    if (perform_decompression(&compressed, output_data, &output_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful. Output size: %zu\n", output_size);
    printf("Original data: ");
    for (size_t i = 0; i < size_input; i++) {
        printf("%d ", input_data[i]);
    }
    printf("\nDecompressed data: ");
    for (size_t i = 0; i < output_size; i++) {
        printf("%d ", output_data[i]);
    }
    printf("\n");
    
    if (memcmp(input_data, output_data, size_input)