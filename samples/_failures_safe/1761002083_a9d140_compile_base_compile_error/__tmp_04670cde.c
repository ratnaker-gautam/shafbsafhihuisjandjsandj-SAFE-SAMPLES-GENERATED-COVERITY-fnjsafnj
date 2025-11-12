//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define MAGIC_HEADER 0xABCDEF00

typedef struct {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
} comp_header_t;

static int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static size_t compress_chunk(const uint8_t *input, uint8_t *output, size_t size) {
    if (size == 0) return 0;
    if (input == NULL || output == NULL) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < size; i += COMPRESSION_RATIO) {
        if (i + 1 < size) {
            uint8_t avg = (uint8_t)((input[i] + input[i + 1]) / 2);
            if (out_idx < MAX_OUTPUT_SIZE) {
                output[out_idx++] = avg;
            }
        }
    }
    return out_idx;
}

static int write_compressed_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size == NULL) return 0;
    
    comp_header_t header;
    header.magic = MAGIC_HEADER;
    header.original_size = (uint32_t)input_size;
    
    size_t compressed_data_size = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_size = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        uint8_t chunk_output[CHUNK_SIZE];
        size_t compressed_chunk = compress_chunk(input + i, chunk_output, chunk_size);
        if (compressed_data_size + compressed_chunk > MAX_OUTPUT_SIZE) return 0;
        if (sizeof(header) + compressed_data_size + compressed_chunk > MAX_OUTPUT_SIZE) return 0;
        memcpy(output + sizeof(header) + compressed_data_size, chunk_output, compressed_chunk);
        compressed_data_size += compressed_chunk;
    }
    
    header.compressed_size = (uint32_t)compressed_data_size;
    memcpy(output, &header, sizeof(header));
    *output_size = sizeof(header) + compressed_data_size;
    return 1;
}

static int read_compressed_data(const uint8_t *input, size_t input_size, uint8_t *output, size_t *output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input_size < sizeof(comp_header_t)) return 0;
    
    comp_header_t header;
    memcpy(&header, input, sizeof(header));
    if (header.magic != MAGIC_HEADER) return 0;
    if (header.original_size > MAX_INPUT_SIZE) return 0;
    if (header.compressed_size > MAX_OUTPUT_SIZE) return 0;
    if (sizeof(header) + header.compressed_size != input_size) return 0;
    
    *output_size = header.original_size;
    size_t data_offset = sizeof(header);
    size_t out_idx = 0;
    
    for (size_t i = 0; i < header.compressed_size; i++) {
        if (out_idx < header.original_size) {
            output[out_idx++] = input[data_offset + i];
            if (out_idx < header.original_size) {
                output[out_idx++] = input[data_offset + i];
            }
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    size_t input_size = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_size < MAX_INPUT_SIZE) {
            input_data[input_size++] = (uint8_t)c;
        }
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    size_t compressed_size = 0;
    if (!write_compressed_data(input_data, input_size, compressed_data, &compressed_size)) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    size_t decompressed_size = 0;
    if (!read_compressed_data(compressed_data, compressed_size, decompressed_data, &decompressed_size)) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (decomp