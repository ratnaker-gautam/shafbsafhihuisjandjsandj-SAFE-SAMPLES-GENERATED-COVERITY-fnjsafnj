//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_DELTA,
    COMPRESS_TYPE_COUNT
};

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return -1;
    if (output_len < input_len * 2) return -1;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_len) return -1;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return (int)out_idx;
}

int compress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return -1;
    if (output_len < input_len + 1) return -1;
    
    output[0] = input[0];
    size_t out_idx = 1;
    
    for (size_t i = 1; i < input_len; i++) {
        if (out_idx >= output_len) return -1;
        int16_t delta = (int16_t)input[i] - (int16_t)input[i-1];
        if (delta < -128 || delta > 127) return -1;
        output[out_idx++] = (uint8_t)(delta + 128);
    }
    
    return (int)out_idx;
}

int decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return -1;
    if (input_len % 2 != 0) return -1;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        uint8_t value = input[in_idx++];
        uint8_t count = input[in_idx++];
        
        if (out_idx + count > output_len) return -1;
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_idx++] = value;
        }
    }
    
    return (int)out_idx;
}

int decompress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return -1;
    if (output_len < input_len - 1) return -1;
    
    output[0] = input[0];
    size_t out_idx = 1;
    
    for (size_t i = 1; i < input_len; i++) {
        if (out_idx >= output_len) return -1;
        int16_t delta = (int16_t)input[i] - 128;
        int16_t reconstructed = (int16_t)output[out_idx-1] + delta;
        if (reconstructed < 0 || reconstructed > 255) return -1;
        output[out_idx++] = (uint8_t)reconstructed;
    }
    
    return (int)out_idx;
}

int main(void) {
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    size_t data_len = sizeof(test_data);
    
    uint8_t compressed[256];
    uint8_t decompressed[256];
    
    printf("Original data: ");
    for (size_t i = 0; i < data_len; i++) {
        printf("%d ", test_data[i]);
    }
    printf("\n");
    
    for (enum compression_type type = COMPRESS_NONE; type < COMPRESS_TYPE_COUNT; type++) {
        int comp_result = -1;
        int decomp_result = -1;
        
        switch (type) {
            case COMPRESS_NONE:
                printf("No compression: ");
                if (sizeof(compressed) >= data_len) {
                    memcpy(compressed, test_data, data_len);
                    comp_result = (int)data_len;
                    memcpy(decompressed, compressed, data_len);
                    decomp_result = (int)data_len;
                }
                break;
                
            case COMPRESS_RLE:
                printf("RLE compression: ");
                comp_result = compress_rle(test_data, data_len, compressed, sizeof(compressed));
                if (comp_result > 0) {
                    decomp_result = decompress_rle(compressed, (size_t)comp_result, decompressed, sizeof(decompressed));
                }
                break;
                
            case COMPRESS_DELTA:
                printf("Delta compression: ");
                comp