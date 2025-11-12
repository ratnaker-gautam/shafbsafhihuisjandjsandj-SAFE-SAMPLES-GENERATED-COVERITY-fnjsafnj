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
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_RATIO) {
        if (i + 1 >= len) break;
        uint8_t high = input[i] >> SHIFT_AMOUNT;
        uint8_t low = input[i + 1] >> SHIFT_AMOUNT;
        CHECK_BOUNDS(out_idx, MAX_OUTPUT_SIZE);
        output[out_idx++] = (high << SHIFT_AMOUNT) | low;
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t *input, size_t len, uint8_t *output) {
    size_t out_idx = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t val = input[i];
        uint8_t high = (val >> SHIFT_AMOUNT) << SHIFT_AMOUNT;
        uint8_t low = (val & (BYTE_MASK >> SHIFT_AMOUNT)) << SHIFT_AMOUNT;
        CHECK_BOUNDS(out_idx + 1, MAX_OUTPUT_SIZE);
        output[out_idx++] = high;
        output[out_idx++] = low;
    }
    return out_idx;
}

static int process_data(int mode, const uint8_t *data, size_t data_len, uint8_t *result, size_t *result_len) {
    if (!data || !result || !result_len) return EXIT_FAILURE;
    if (data_len == 0 || data_len > MAX_INPUT_SIZE) return EXIT_FAILURE;
    
    size_t len = 0;
    if (mode == 0) {
        len = compress_chunk(data, data_len, result);
    } else if (mode == 1) {
        len = decompress_chunk(data, data_len, result);
    } else {
        return EXIT_FAILURE;
    }
    
    if (len == 0 || len > MAX_OUTPUT_SIZE) return EXIT_FAILURE;
    *result_len = len;
    return EXIT_SUCCESS;
}

static int read_input(uint8_t *buffer, size_t *length) {
    if (!buffer || !length) return EXIT_FAILURE;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    size_t size;
    if (scanf("%zu", &size) != 1) return EXIT_FAILURE;
    if (size == 0 || size > MAX_INPUT_SIZE) return EXIT_FAILURE;
    
    printf("Enter %zu bytes as hexadecimal values (space separated): ", size);
    for (size_t i = 0; i < size; ++i) {
        unsigned int val;
        if (scanf("%02x", &val) != 1) return EXIT_FAILURE;
        if (val > UINT8_MAX) return EXIT_FAILURE;
        buffer[i] = (uint8_t)val;
    }
    *length = size;
    return EXIT_SUCCESS;
}

static void print_output(const uint8_t *data, size_t length) {
    if (!data || length == 0) return;
    printf("Result (%zu bytes): ", length);
    for (size_t i = 0; i < length; ++i) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    uint8_t output[MAX_OUTPUT_SIZE];
    size_t input_len = 0;
    size_t output_len = 0;
    
    printf("Compression/Decompression Tool\n");
    printf("0: Compress, 1: Decompress\n");
    printf("Select mode: ");
    
    int mode;
    if (scanf("%d", &mode) != 1) {
        printf("Invalid mode selection\n");
        return EXIT_FAILURE;
    }
    
    if (mode != 0 && mode != 1) {
        printf("Invalid mode\n");
        return EXIT_FAILURE;
    }
    
    if (read_input(input, &input_len) != EXIT_SUCCESS) {
        printf("Input error\n");
        return EXIT_FAILURE;
    }
    
    if (process_data(mode, input, input_len, output, &output_len) != EXIT_SUCCESS) {
        printf("Processing error\n");
        return EXIT_FAILURE;
    }
    
    print_output(output, output_len);
    return EXIT_SUCCESS;
}