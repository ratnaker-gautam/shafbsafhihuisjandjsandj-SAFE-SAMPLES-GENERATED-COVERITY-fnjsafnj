//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_FACTOR 2
#define CHUNK_SIZE 8
#define BYTE_MASK 0xFF
#define SHIFT_AMOUNT 4

#define VALIDATE_PTR(ptr) if (!(ptr)) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { return EXIT_FAILURE; }

static size_t compress_chunk(const uint8_t* input, uint8_t* output, size_t len) {
    if (len == 0) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i += COMPRESSION_FACTOR) {
        if (i + 1 < len) {
            uint8_t high = (input[i] >> SHIFT_AMOUNT) & 0x0F;
            uint8_t low = (input[i + 1] >> SHIFT_AMOUNT) & 0x0F;
            output[out_idx++] = (high << SHIFT_AMOUNT) | low;
        } else {
            output[out_idx++] = input[i] & BYTE_MASK;
        }
    }
    return out_idx;
}

static size_t decompress_chunk(const uint8_t* input, uint8_t* output, size_t len) {
    if (len == 0) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t val = input[i];
        output[out_idx++] = (val & 0xF0) | ((val & 0x0F) << SHIFT_AMOUNT);
        output[out_idx++] = ((val & 0x0F) << SHIFT_AMOUNT) | (val & 0x0F);
        if (out_idx >= MAX_OUTPUT_SIZE) break;
    }
    return out_idx;
}

static int process_data(int compress, const uint8_t* input, size_t input_len, uint8_t* output, size_t* output_len) {
    if (!input || !output || !output_len) return EXIT_FAILURE;
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) return EXIT_FAILURE;
    
    size_t result_len;
    if (compress) {
        result_len = compress_chunk(input, output, input_len);
    } else {
        result_len = decompress_chunk(input, output, input_len);
    }
    
    if (result_len == 0 || result_len > MAX_OUTPUT_SIZE) return EXIT_FAILURE;
    *output_len = result_len;
    return EXIT_SUCCESS;
}

int main(void) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t output_buffer[MAX_OUTPUT_SIZE];
    size_t input_len = 0;
    size_t output_len = 0;
    
    int mode;
    if (scanf("%d", &mode) != 1) {
        return EXIT_FAILURE;
    }
    if (mode != 0 && mode != 1) {
        return EXIT_FAILURE;
    }
    
    if (scanf("%zu", &input_len) != 1) {
        return EXIT_FAILURE;
    }
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; ++i) {
        int byte_val;
        if (scanf("%d", &byte_val) != 1) {
            return EXIT_FAILURE;
        }
        if (byte_val < 0 || byte_val > UINT8_MAX) {
            return EXIT_FAILURE;
        }
        input_buffer[i] = (uint8_t)byte_val;
    }
    
    if (process_data(mode, input_buffer, input_len, output_buffer, &output_len) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    printf("%zu\n", output_len);
    for (size_t i = 0; i < output_len; ++i) {
        printf("%d ", output_buffer[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}