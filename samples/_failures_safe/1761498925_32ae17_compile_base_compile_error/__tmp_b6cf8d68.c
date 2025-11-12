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
        if (i % 2 == 0) {
            output[i / 2] = (input[i] & 0xF0) | ((i + 1 < len ? input[i + 1] : 0) & 0x0F);
        }
    }
    return (len + 1) / 2;
}

static int decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    if (!input || !output || len == 0 || len > CHUNK_SIZE / 2) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        output[i * 2] = input[i] & 0xF0;
        if (i * 2 + 1 < CHUNK_SIZE) {
            output[i * 2 + 1] = (input[i] & 0x0F) << 4;
        }
    }
    return len * 2;
}

static int compress_data(const uint8_t *input, size_t input_len, compressed_data_t *compressed) {
    if (!input || !compressed || input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return -1;
    }
    
    compressed->header = MAGIC_HEADER;
    compressed->original_size = (uint32_t)input_len;
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i += CHUNK_SIZE) {
        size_t chunk_len = MIN(CHUNK_SIZE, input_len - i);
        int result = compress_chunk(input + i, chunk_len, compressed->data + output_idx);
        if (result < 0) {
            return -1;
        }
        output_idx += (size_t)result;
        CHECK_BOUNDS(output_idx, MAX_OUTPUT_SIZE);
    }
    
    compressed->compressed_size = (uint32_t)output_idx;
    return 0;
}

static int decompress_data(const compressed_data_t *compressed, uint8_t *output, size_t output_size) {
    if (!compressed || !output || compressed->header != MAGIC_HEADER) {
        return -1;
    }
    
    if (compressed->original_size > output_size || compressed->original_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < compressed->compressed_size; ) {
        size_t remaining = compressed->compressed_size - i;
        size_t chunk_len = MIN(CHUNK_SIZE / 2, remaining);
        int result = decompress_chunk(compressed->data + i, chunk_len, output + output_idx);
        if (result < 0) {
            return -1;
        }
        i += chunk_len;
        output_idx += (size_t)result;
        CHECK_BOUNDS(output_idx, output_size);
    }
    
    if (output_idx != compressed->original_size) {
        return -1;
    }
    
    return 0;
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
            printf("Input too long\n");
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
    printf("Compressed size: %u bytes\n", compressed.compressed_size);
    printf("Compression ratio: %.2f:1\n", 
           (float)compressed.original_size / (float)compressed.compressed_size);
    
    if (decompress_data(&compressed, output_data, sizeof