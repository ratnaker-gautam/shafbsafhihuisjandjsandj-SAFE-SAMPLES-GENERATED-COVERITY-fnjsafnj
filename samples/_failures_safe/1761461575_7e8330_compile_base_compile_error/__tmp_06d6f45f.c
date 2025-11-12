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
#define CHECK_BOUNDS(idx, max) if ((idx) < 0 || (idx) >= (max)) { return EXIT_FAILURE; }

typedef struct {
    uint32_t magic;
    uint32_t original_size;
} CompressionHeader;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t compress_data(const char* input, size_t input_len, char* output, size_t output_len) {
    if (!validate_input(input, input_len)) return 0;
    if (output == NULL || output_len < input_len * COMPRESSED_MULTIPLIER) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (out_idx + 2 > output_len) return 0;
        output[out_idx++] = input[i];
        output[out_idx++] = input[i];
    }
    return out_idx;
}

size_t decompress_data(const char* input, size_t input_len, char* output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len % 2 != 0) return 0;
    
    size_t expected_output = input_len / 2;
    if (output_len < expected_output) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        if (i + 1 >= input_len) return 0;
        if (input[i] != input[i + 1]) return 0;
        output[out_idx++] = input[i];
    }
    return out_idx;
}

int write_compressed_file(const char* filename, const char* data, size_t data_len) {
    if (filename == NULL || data == NULL) return EXIT_FAILURE;
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return EXIT_FAILURE;
    
    CompressionHeader header;
    header.magic = MAGIC_NUMBER;
    header.original_size = (uint32_t)data_len;
    
    if (fwrite(&header, sizeof(header), 1, file) != 1) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    char* compressed = malloc(data_len * COMPRESSED_MULTIPLIER);
    CHECK_NULL(compressed);
    
    size_t compressed_size = compress_data(data, data_len, compressed, data_len * COMPRESSED_MULTIPLIER);
    if (compressed_size == 0) {
        free(compressed);
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (fwrite(compressed, 1, compressed_size, file) != compressed_size) {
        free(compressed);
        fclose(file);
        return EXIT_FAILURE;
    }
    
    free(compressed);
    fclose(file);
    return EXIT_SUCCESS;
}

int read_compressed_file(const char* filename, char** output, size_t* output_len) {
    if (filename == NULL || output == NULL || output_len == NULL) return EXIT_FAILURE;
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return EXIT_FAILURE;
    
    CompressionHeader header;
    if (fread(&header, sizeof(header), 1, file) != 1) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (header.magic != MAGIC_NUMBER) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    if (header.original_size == 0 || header.original_size > MAX_INPUT_SIZE) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, HEADER_SIZE, SEEK_SET);
    
    if (file_size < HEADER_SIZE) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    size_t compressed_size = file_size - HEADER_SIZE;
    if (compressed_size > MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER) {
        fclose(file);
        return EXIT_FAILURE;
    }
    
    char* compressed_data = malloc(compressed_size);
    CHECK_NULL(compressed_data);
    
    if (fread(compressed_data, 1, compressed_size, file) != compressed_size) {
        free(compressed_data);
        fclose(file);
        return EXIT_FAILURE;
    }
    
    char* decompressed = malloc(header.original_size);
    CHECK_NULL(decompressed);
    
    size_t decompressed_size = decompress_data(compressed_data, compressed_size, decompressed, header.original_size);
    if (decompressed_size != header.original_size) {
        free(compressed_data);