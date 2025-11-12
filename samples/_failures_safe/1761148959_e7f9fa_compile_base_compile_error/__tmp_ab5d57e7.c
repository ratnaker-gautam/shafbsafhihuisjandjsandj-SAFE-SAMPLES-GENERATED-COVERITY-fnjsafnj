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
#define MAGIC_HEADER 0xABCDEF00

typedef struct {
    uint32_t header;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t data[MAX_OUTPUT_SIZE];
} compressed_data_t;

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (!input || !output || len == 0 || len > CHUNK_SIZE) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (i % COMPRESSION_RATIO == 0) {
            output[i / COMPRESSION_RATIO] = input[i];
        }
    }
    
    return (len + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output, size_t original_len) {
    if (!input || !output || len == 0 || original_len == 0 || original_len > CHUNK_SIZE) {
        return -1;
    }
    
    for (size_t i = 0; i < original_len; i++) {
        if (i % COMPRESSION_RATIO == 0) {
            output[i] = input[i / COMPRESSION_RATIO];
        } else {
            output[i] = 0;
        }
    }
    
    return original_len;
}

static int compress_data(const uint8_t *input, size_t input_len, compressed_data_t *compressed) {
    if (!input || !compressed || input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return -1;
    }
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint32_t)input_len;
    
    size_t total_compressed = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = MIN(CHUNK_SIZE, input_len - i);
        int compressed_len = compress_chunk(input + i, chunk_len, 
                                          compressed->data + total_compressed);
        if (compressed_len < 0) {
            return -1;
        }
        total_compressed += compressed_len;
        CHECK_BOUNDS(total_compressed, MAX_OUTPUT_SIZE);
    }
    
    compressed->compressed_size = (uint32_t)total_compressed;
    return 0;
}

static int decompress_data(const compressed_data_t *compressed, uint8_t *output, size_t output_size) {
    if (!compressed || !output || compressed->header != MAGIC_HEADER) {
        return -1;
    }
    
    if (compressed->original_size > output_size || compressed->original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t total_decompressed = 0;
    size_t compressed_pos = 0;
    
    while (total_decompressed < compressed->original_size) {
        size_t remaining = compressed->original_size - total_decompressed;
        size_t chunk_len = MIN(CHUNK_SIZE, remaining);
        
        int decompressed_len = decompress_chunk(compressed->data + compressed_pos,
                                               compressed->compressed_size - compressed_pos,
                                               output + total_decompressed,
                                               chunk_len);
        if (decompressed_len < 0) {
            return -1;
        }
        
        total_decompressed += decompressed_len;
        compressed_pos += (chunk_len + COMPRESSION_RATIO - 1) / COMPRESSION_RATIO;
        
        CHECK_BOUNDS(compressed_pos, compressed->compressed_size);
        CHECK_BOUNDS(total_decompressed, output_size);
    }
    
    return total_decompressed == compressed->original_size ? 0 : -1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    compressed_data_t compressed;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t input_len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (input_len >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return EXIT_FAILURE;
        }
        input_data[input_len++] = (uint8_t)c;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    if (compress_data(input_data, input_len, &compressed) != 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %u bytes\n", compressed.original_size);
    printf("Compressed size: %u bytes\n", compressed.compressed