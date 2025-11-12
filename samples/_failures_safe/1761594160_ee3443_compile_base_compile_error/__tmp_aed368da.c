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

#define CHECK_BOUNDS(ptr, size, max) do { \
    if ((size) > (max) || (ptr) == NULL) { \
        return -1; \
    } \
} while(0)

#define VALIDATE_INPUT(input, len) do { \
    if ((input) == NULL || (len) == 0 || (len) > MAX_INPUT_SIZE) { \
        return -1; \
    } \
} while(0)

#define WRITE_HEADER(buf, magic, size) do { \
    uint32_t *header = (uint32_t *)(buf); \
    header[0] = (magic); \
    header[1] = (size); \
} while(0)

#define READ_HEADER(buf, magic, size) do { \
    const uint32_t *header = (const uint32_t *)(buf); \
    (magic) = header[0]; \
    (size) = header[1]; \
} while(0)

int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    VALIDATE_INPUT(input, input_len);
    CHECK_BOUNDS(output, *output_len, MAX_OUTPUT_SIZE);
    
    if (*output_len < HEADER_SIZE + input_len / COMPRESSION_FACTOR) {
        return -1;
    }
    
    WRITE_HEADER(output, MAGIC_NUMBER, input_len);
    
    size_t out_idx = HEADER_SIZE;
    for (size_t i = 0; i < input_len; i += COMPRESSION_FACTOR) {
        if (out_idx >= *output_len) {
            return -1;
        }
        uint8_t compressed_byte = 0;
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < input_len; j++) {
            compressed_byte |= (input[i + j] & 0xC0) >> (2 * j);
        }
        output[out_idx++] = compressed_byte;
    }
    
    *output_len = out_idx;
    return 0;
}

int decompress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    VALIDATE_INPUT(input, input_len);
    CHECK_BOUNDS(output, *output_len, MAX_OUTPUT_SIZE);
    
    if (input_len < HEADER_SIZE) {
        return -1;
    }
    
    uint32_t magic, original_size;
    READ_HEADER(input, magic, original_size);
    
    if (magic != MAGIC_NUMBER || original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    if (*output_len < original_size) {
        return -1;
    }
    
    size_t in_idx = HEADER_SIZE;
    for (size_t i = 0; i < original_size; i += COMPRESSION_FACTOR) {
        if (in_idx >= input_len) {
            return -1;
        }
        uint8_t compressed_byte = input[in_idx++];
        for (size_t j = 0; j < COMPRESSION_FACTOR && i + j < original_size; j++) {
            uint8_t shift = 2 * j;
            output[i + j] = (compressed_byte << shift) & 0xC0;
        }
    }
    
    *output_len = original_size;
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    char buffer[MAX_INPUT_SIZE * 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }
    
    memcpy(input_data, buffer, input_len);
    
    size_t compressed_len = sizeof(compressed_data);
    if (compress_data(input_data, input_len, compressed_data, &compressed_len) != 0) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    
    size_t decompressed_len = sizeof(decompressed_data);
    if (decompress_data(compressed_data, compressed_len, decompressed_data, &decompressed_len) != 0) {
        fprintf(stderr, "Decomp