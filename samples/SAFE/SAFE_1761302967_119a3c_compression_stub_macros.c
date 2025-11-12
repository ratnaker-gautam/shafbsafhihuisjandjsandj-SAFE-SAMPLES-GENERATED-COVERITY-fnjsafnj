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
#define VALIDATE_SIZE(sz) if ((sz) <= 0 || (sz) > MAX_INPUT_SIZE) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int compress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    CompressionHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)input_size;
    
    if (sizeof(header) + (input_size + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO > MAX_OUTPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    memcpy(output, &header, sizeof(header));
    size_t out_idx = sizeof(header);
    
    for (size_t i = 0; i < input_size; i += COMPRESSION_RATIO) {
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < input_size; j++) {
            compressed_byte ^= input[i + j];
        }
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        output[out_idx++] = compressed_byte;
    }
    
    *output_size = out_idx;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    if (input_size < sizeof(CompressionHeader)) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader header;
    memcpy(&header, input, sizeof(header));
    
    if (header.magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    
    if (header.original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = input_size - sizeof(header);
    size_t expected_compressed_size = (header.original_size + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
    
    if (compressed_size != expected_compressed_size) {
        return EXIT_FAILURE;
    }
    
    *output_size = header.original_size;
    
    for (size_t i = 0; i < header.original_size; i++) {
        size_t block_idx = i / COMPRESSION_RATIO;
        CHECK_BOUNDS(block_idx, compressed_size);
        output[i] = input[sizeof(header) + block_idx];
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char input_buffer[MAX_INPUT_SIZE * 2];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, input_buffer, input_len);
    size_t input_size = input_len;
    
    size_t compressed_size;
    if (compress_data(input_data, input_size, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_data, decompressed_data, input_size) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful: ");
    for (size_t i = 0; i < decompressed_size && i < 32; i++) {
        printf("%c", decompressed_data[i]);
    }
    if (decompressed_size > 32) {
        printf("...");
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}