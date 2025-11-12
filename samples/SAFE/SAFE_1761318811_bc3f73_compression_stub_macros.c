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
#define MAGIC_HEADER 0xDEADBEEF

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE - 8];
} compressed_data_t;

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || input == NULL || output == NULL) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += CHUNK_SIZE) {
        uint8_t chunk[CHUNK_SIZE] = {0};
        size_t chunk_len = (i + CHUNK_SIZE <= len) ? CHUNK_SIZE : len - i;
        memcpy(chunk, input + i, chunk_len);
        output[out_idx++] = (uint8_t)(chunk_len * COMPRESSION_RATIO);
        for (size_t j = 0; j < chunk_len; j++) {
            output[out_idx++] = chunk[j] ^ 0xAA;
        }
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || input == NULL || output == NULL) return 0;
    size_t out_idx = 0;
    size_t in_idx = 0;
    while (in_idx < len) {
        uint8_t chunk_size = input[in_idx++];
        if (chunk_size == 0 || chunk_size > CHUNK_SIZE * COMPRESSION_RATIO) break;
        size_t actual_chunk = chunk_size / COMPRESSION_RATIO;
        if (in_idx + actual_chunk > len) break;
        for (size_t j = 0; j < actual_chunk; j++) {
            if (out_idx >= MAX_INPUT_SIZE) break;
            output[out_idx++] = input[in_idx + j] ^ 0xAA;
        }
        in_idx += actual_chunk;
    }
    return out_idx;
}

static int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    compressed_data_t comp_data;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input data\n");
        return EXIT_FAILURE;
    }
    
    comp_data.header = MAGIC_HEADER;
    comp_data.original_size = (uint16_t)input_len;
    
    size_t compressed_size = compress_chunk(input_buffer, input_len, comp_data.data);
    if (compressed_size == 0 || compressed_size > sizeof(comp_data.data)) {
        fprintf(stderr, "Compression failed\n");
        return EXIT_FAILURE;
    }
    
    comp_data.compressed_size = (uint16_t)compressed_size;
    
    printf("Original size: %u bytes\n", comp_data.original_size);
    printf("Compressed size: %u bytes\n", comp_data.compressed_size);
    printf("Compression ratio: %.2f:1\n", 
           (float)comp_data.original_size / comp_data.compressed_size);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress_chunk(comp_data.data, compressed_size, decompressed);
    
    if (decompressed_size != input_len || memcmp(input_buffer, decompressed, input_len) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Decompression verified successfully\n");
    printf("Original: %s\n", input_buffer);
    printf("Decompressed: %s\n", decompressed);
    
    return EXIT_SUCCESS;
}