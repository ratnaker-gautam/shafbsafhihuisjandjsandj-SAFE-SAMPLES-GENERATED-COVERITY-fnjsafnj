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
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len > CHUNK_SIZE) return -1;
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ 0xAA;
    }
    return (int)len;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len > CHUNK_SIZE) return -1;
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ 0xAA;
    }
    return (int)len;
}

int compress_data(const uint8_t *input, size_t input_size, compressed_data_t *compressed) {
    VALIDATE_SIZE(input_size);
    CHECK_NULL(input);
    CHECK_NULL(compressed);
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint16_t)input_size;
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_size; i += CHUNK_SIZE) {
        size_t chunk_len = (input_size - i) < CHUNK_SIZE ? (input_size - i) : CHUNK_SIZE;
        int result = compress_chunk(input + i, chunk_len, compressed->data + output_idx);
        if (result < 0) return EXIT_FAILURE;
        output_idx += (size_t)result;
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
    }
    
    compressed->compressed_size = (uint16_t)output_idx;
    return EXIT_SUCCESS;
}

int decompress_data(const compressed_data_t *compressed, uint8_t *output, size_t output_size) {
    CHECK_NULL(compressed);
    CHECK_NULL(output);
    if (compressed->header != MAGIC_HEADER) return EXIT_FAILURE;
    if (compressed->original_size > output_size) return EXIT_FAILURE;
    
    size_t output_idx = 0;
    for (size_t i = 0; i < compressed->compressed_size; i += CHUNK_SIZE) {
        size_t chunk_len = (compressed->compressed_size - i) < CHUNK_SIZE ? (compressed->compressed_size - i) : CHUNK_SIZE;
        int result = decompress_chunk(compressed->data + i, chunk_len, output + output_idx);
        if (result < 0) return EXIT_FAILURE;
        output_idx += (size_t)result;
        CHECK_BOUNDS(output_idx, output_size);
    }
    
    if (output_idx != compressed->original_size) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    
    if (compress_data(input_data, input_len, &compressed) != EXIT_SUCCESS) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %u bytes\n", compressed.original_size);
    printf("Compressed size: %u bytes\n", compressed.compressed_size);
    
    if (decompress_data(&compressed, output_data, MAX_INPUT_SIZE) != EXIT_SUCCESS) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    output_data[compressed.original_size] = '\0';
    printf("Decompressed data: %s\n", output_data);
    
    if (memcmp(input_data, output_data, compressed.original_size) != 0) {
        printf("Data mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression/decompression successful\n");
    return EXIT_SUCCESS;
}