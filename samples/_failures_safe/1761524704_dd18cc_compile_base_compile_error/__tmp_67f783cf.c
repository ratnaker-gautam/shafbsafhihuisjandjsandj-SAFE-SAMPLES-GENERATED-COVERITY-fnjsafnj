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

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    uint32_t header;
    uint16_t original_size;
    uint16_t compressed_size;
    uint8_t data[];
} compressed_data_t;

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || !input || !output) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        uint8_t compressed = 0;
        for (size_t j = 0; j < COMPRESSION_RATIO && (i + j) < len; j++) {
            compressed = (compressed << 4) | (input[i + j] & 0x0F);
        }
        output[out_idx++] = compressed;
    }
    return out_idx;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (len == 0 || !input || !output) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t byte = input[i];
        for (int j = COMPRESSION_RATIO - 1; j >= 0; j--) {
            if (out_idx < MAX_OUTPUT_SIZE) {
                output[out_idx++] = (byte >> (j * 4)) & 0x0F;
            }
        }
    }
    return out_idx;
}

static int perform_compression(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (!input || !output || !output_len || input_len == 0) return EXIT_FAILURE;
    if (input_len > MAX_INPUT_SIZE) return EXIT_FAILURE;
    
    compressed_data_t *compressed = (compressed_data_t *)output;
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint16_t)input_len;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = MIN(CHUNK_SIZE, input_len - i);
        int result = compress_chunk(input + i, chunk_len, compressed->data + compressed_size);
        if (result < 0) return EXIT_FAILURE;
        compressed_size += result;
    }
    
    if (compressed_size > UINT16_MAX) return EXIT_FAILURE;
    compressed->compressed_size = (uint16_t)compressed_size;
    *output_len = sizeof(compressed_data_t) + compressed_size;
    return EXIT_SUCCESS;
}

static int perform_decompression(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (!input || !output || !output_len || input_len < sizeof(compressed_data_t)) return EXIT_FAILURE;
    
    const compressed_data_t *compressed = (const compressed_data_t *)input;
    if (compressed->header != MAGIC_HEADER) return EXIT_FAILURE;
    if (compressed->original_size > MAX_INPUT_SIZE) return EXIT_FAILURE;
    if (compressed->compressed_size > MAX_OUTPUT_SIZE) return EXIT_FAILURE;
    
    size_t total_decompressed = 0;
    for (size_t i = 0; i < compressed->compressed_size; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        size_t chunk_len = MIN(CHUNK_SIZE / COMPRESSION_RATIO, compressed->compressed_size - i);
        int result = decompress_chunk(compressed->data + i, chunk_len, output + total_decompressed);
        if (result < 0) return EXIT_FAILURE;
        total_decompressed += result;
    }
    
    if (total_decompressed != compressed->original_size) return EXIT_FAILURE;
    *output_len = total_decompressed;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    char buffer[MAX_INPUT_SIZE * 2];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {