//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_DELTA,
    COMPRESS_TYPE_COUNT
};

int compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size < input_len) {
        return -1;
    }
    memcpy(output, input, input_len);
    return (int)input_len;
}

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0) {
        return -1;
    }
    size_t out_idx = 0;
    size_t i = 0;
    while (i < input_len) {
        if (out_idx + 2 > output_size) {
            return -1;
        }
        uint8_t value = input[i];
        size_t count = 1;
        while (i + count < input_len && count < 255 && input[i + count] == value) {
            count++;
        }
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = value;
        i += count;
    }
    return (int)out_idx;
}

int compress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size < input_len) {
        return -1;
    }
    output[0] = input[0];
    for (size_t i = 1; i < input_len; i++) {
        int16_t delta = (int16_t)input[i] - (int16_t)input[i - 1];
        if (delta < -128 || delta > 127) {
            return -1;
        }
        output[i] = (uint8_t)(delta + 128);
    }
    return (int)input_len;
}

int decompress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || output_size < input_len) {
        return -1;
    }
    memcpy(output, input, input_len);
    return (int)input_len;
}

int decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0) {
        return -1;
    }
    size_t out_idx = 0;
    size_t i = 0;
    while (i < input_len) {
        if (i + 1 >= input_len) {
            return -1;
        }
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        if (out_idx + count > output_size) {
            return -1;
        }
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    return (int)out_idx;
}

int decompress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size < input_len) {
        return -1;
    }
    output[0] = input[0];
    for (size_t i = 1; i < input_len; i++) {
        int16_t delta = (int16_t)input[i] - 128;
        int16_t reconstructed = (int16_t)output[i - 1] + delta;
        if (reconstructed < 0 || reconstructed > 255) {
            return -1;
        }
        output[i] = (uint8_t)reconstructed;
    }
    return (int)input_len;
}

int main(void) {
    uint8_t test_data[] = {10, 10, 10, 20, 30, 30, 40};
    size_t data_len = sizeof(test_data);
    uint8_t compressed[256];
    uint8_t decompressed[256];
    
    for (enum compression_type type = COMPRESS_NONE; type < COMPRESS_TYPE_COUNT; type++) {
        int comp_result;
        switch (type) {
            case COMPRESS_NONE:
                comp_result = compress_none(test_data, data_len, compressed, sizeof(compressed));
                break;
            case COMPRESS_RLE:
                comp_result = compress_rle(test_data, data_len, compressed, sizeof(compressed));
                break;
            case COMPRESS_DELTA:
                comp_result = compress_delta(test_data, data_len, compressed, sizeof(compressed));
                break;
            default:
                continue;
        }
        
        if (comp_result < 0) {
            printf("Compression failed for type %d\n", type);
            continue;
        }
        
        int decomp_result;
        switch (type) {
            case COMPRESS_NONE:
                decomp_result = decompress_none(compressed, (size_t)comp_result, decompressed, sizeof(decompressed));
                break;
            case COMPRESS_RLE:
                decomp_result = decompress_rle(compressed, (size_t)