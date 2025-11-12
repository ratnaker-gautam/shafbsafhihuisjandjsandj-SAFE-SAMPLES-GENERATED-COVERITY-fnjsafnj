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
    
    CompressionHeader* header = (CompressionHeader*)output;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    size_t output_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_size; i += COMPRESSION_FACTOR) {
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR; j++) {
            if (i + j < input_size) {
                compressed_byte ^= input[i + j];
            }
        }
        output[output_idx++] = compressed_byte;
    }
    
    *output_size = output_idx;
    return EXIT_SUCCESS;
}

int decompress_data(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) {
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(input_size);
    
    if (input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    CompressionHeader* header = (CompressionHeader*)input;
    if (header->magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    
    size_t original_size = header->original_size;
    if (original_size <= 0 || original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = input_size - HEADER_SIZE;
    size_t expected_compressed = (original_size + COMPRESSION_FACTOR - 1) / COMPRESSION_FACTOR;
    if (compressed_size != expected_compressed) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < original_size; i++) {
        CHECK_BOUNDS(i, MAX_INPUT_SIZE);
        size_t block_idx = i / COMPRESSION_FACTOR;
        CHECK_BOUNDS(block_idx, compressed_size);
        output[i] = input[HEADER_SIZE + block_idx];
    }
    
    *output_size = original_size;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    char buffer[MAX_INPUT_SIZE * 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, buffer, input_len);
    
    size_t compressed_size;
    if (compress_data(input_data, input_len, compressed_data, &compressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_len, compressed_size);
    
    size_t decompressed_size;
    if (decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_len || memcmp(input_data, decompressed_data, input_len) != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful: ");
    fwrite(decompressed_data, 1, decompressed_size, stdout);
    printf("\n");
    
    return EXIT_SUCCESS;
}