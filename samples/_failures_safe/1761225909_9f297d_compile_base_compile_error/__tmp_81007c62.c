//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define HEADER_SIZE 8
#define MAGIC_NUMBER 0xDEADBEEF

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressedHeader;

static int compress_data(const uint8_t *input, size_t input_size, uint8_t **output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    size_t compressed_buffer_size = input_size * COMPRESSED_MULTIPLIER + HEADER_SIZE;
    if (compressed_buffer_size > SIZE_MAX / sizeof(uint8_t)) {
        return EXIT_FAILURE;
    }
    
    uint8_t *compressed_data = malloc(compressed_buffer_size);
    CHECK_NULL(compressed_data);
    
    CompressedHeader *header = (CompressedHeader *)compressed_data;
    header->magic = MAGIC_NUMBER;
    header->original_size = (uint32_t)input_size;
    
    for (size_t i = 0; i < input_size; i++) {
        CHECK_BOUNDS(i + HEADER_SIZE, compressed_buffer_size);
        compressed_data[i + HEADER_SIZE] = input[i] ^ 0x55;
    }
    
    *output_size = input_size + HEADER_SIZE;
    *output = compressed_data;
    return EXIT_SUCCESS;
}

static int decompress_data(const uint8_t *input, size_t input_size, uint8_t **output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL || input_size < HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    const CompressedHeader *header = (const CompressedHeader *)input;
    if (header->magic != MAGIC_NUMBER) {
        return EXIT_FAILURE;
    }
    
    if (header->original_size == 0 || header->original_size > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    if (input_size != header->original_size + HEADER_SIZE) {
        return EXIT_FAILURE;
    }
    
    uint8_t *decompressed_data = malloc(header->original_size);
    CHECK_NULL(decompressed_data);
    
    for (size_t i = 0; i < header->original_size; i++) {
        CHECK_BOUNDS(i + HEADER_SIZE, input_size);
        decompressed_data[i] = input[i + HEADER_SIZE] ^ 0x55;
    }
    
    *output_size = header->original_size;
    *output = decompressed_data;
    return EXIT_SUCCESS;
}

static int read_input(uint8_t *buffer, size_t max_size, size_t *actual_size) {
    if (buffer == NULL || actual_size == NULL || max_size == 0) {
        return EXIT_FAILURE;
    }
    
    printf("Enter data to compress (max %zu bytes): ", max_size);
    fflush(stdout);
    
    if (fgets((char *)buffer, (int)max_size, stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    *actual_size = strlen((char *)buffer);
    if (*actual_size > 0 && buffer[*actual_size - 1] == '\n') {
        buffer[*actual_size - 1] = '\0';
        (*actual_size)--;
    }
    
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    if (read_input(input_buffer, sizeof(input_buffer), &input_size) != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to read input\n");
        return EXIT_FAILURE;
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    uint8_t *compressed_data = NULL;
    size_t compressed_size = 0;
    
    if (compress_data(input_buffer, input_size, &compressed_data, &compressed_size) != EXIT_SUCCESS) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_size, compressed_size);
    
    uint8_t *decompressed_data = NULL;
    size_t decompressed_size = 0;
    
    if (decompress_data(compressed_data, compressed_size, &decompressed_data, &decompressed_size) != EXIT_SUCCESS) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed_data);
        return EXIT_FAILURE;
    }
    
    if (decompressed_size != input_size || memcmp(input_buffer, decompressed_data, input_size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        free(compressed_data);
        free(decompressed_data);
        return EXIT_FAILURE;
    }
    
    printf("Decompression successful: '");
    for (