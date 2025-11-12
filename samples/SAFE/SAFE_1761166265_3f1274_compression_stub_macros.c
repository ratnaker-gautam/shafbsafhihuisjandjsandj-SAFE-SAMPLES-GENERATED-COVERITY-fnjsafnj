//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER)

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!input || !output || input_len == 0 || output_max < input_len * 2) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        CHECK_BOUNDS(output_idx, output_max - 1);
        uint8_t current = input[i];
        output[output_idx++] = (current >> SHIFT_AMOUNT) | 0x40;
        output[output_idx++] = (current & (BYTE_MASK >> SHIFT_AMOUNT)) | 0x40;
    }
    
    return output_idx;
}

static size_t decompress_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!input || !output || input_len == 0 || (input_len % 2) != 0 || output_max < input_len / 2) {
        return 0;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        CHECK_BOUNDS(i + 1, input_len);
        CHECK_BOUNDS(output_idx, output_max);
        uint8_t high = input[i] & ~0x40;
        uint8_t low = input[i + 1] & ~0x40;
        output[output_idx++] = (high << SHIFT_AMOUNT) | low;
    }
    
    return output_idx;
}

static int compare_data(const uint8_t* data1, const uint8_t* data2, size_t len) {
    if (!data1 || !data2) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (data1[i] != data2[i]) {
            return -1;
        }
    }
    
    return 0;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_COMPRESSED_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    fflush(stdout);
    
    size_t input_len = 0;
    int current_char;
    while ((current_char = getchar()) != EOF && current_char != '\n') {
        if (input_len >= MAX_INPUT_SIZE) {
            printf("Input too long\n");
            return EXIT_FAILURE;
        }
        input_data[input_len++] = (uint8_t)current_char;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    size_t compressed_len = compress_data(input_data, input_len, compressed_data, MAX_COMPRESSED_SIZE);
    if (compressed_len == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_len);
    
    size_t decompressed_len = decompress_data(compressed_data, compressed_len, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_len == 0) {
        printf("Decompression failed\n");
        return EXIT_FAILURE;
    }
    
    if (decompressed_len != input_len) {
        printf("Size mismatch after decompression\n");
        return EXIT_FAILURE;
    }
    
    if (compare_data(input_data, decompressed_data, input_len) != 0) {
        printf("Data corruption detected\n");
        return EXIT_FAILURE;
    }
    
    printf("Compression and decompression successful\n");
    printf("Compression ratio: %.2f%%\n", ((float)compressed_len / input_len) * 100.0f);
    
    return EXIT_SUCCESS;
}