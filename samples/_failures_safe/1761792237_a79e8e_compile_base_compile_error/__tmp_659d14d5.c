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

int compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size == 0) {
        return -1;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (out_index + 2 > output_size) {
            return -1;
        }
        
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_index++] = count;
        output[out_index++] = current;
        i += count;
    }
    
    return (int)out_index;
}

int compress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size < input_len) {
        return -1;
    }
    
    output[0] = input[0];
    
    for (size_t i = 1; i < input_len; i++) {
        if (i >= output_size) {
            return -1;
        }
        int16_t delta = (int16_t)input[i] - (int16_t)input[i - 1];
        if (delta < -128 || delta > 127) {
            return -1;
        }
        output[i] = (uint8_t)(delta + 128);
    }
    
    return (int)input_len;
}

int decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size == 0) {
        return -1;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (i + 1 >= input_len) {
            return -1;
        }
        
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_index + count > output_size) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_index++] = value;
        }
    }
    
    return (int)out_index;
}

int decompress_delta(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_len == 0 || output_size < input_len) {
        return -1;
    }
    
    output[0] = input[0];
    
    for (size_t i = 1; i < input_len; i++) {
        if (i >= output_size) {
            return -1;
        }
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
    uint8_t input_data[256];
    uint8_t compressed[512];
    uint8_t decompressed[256];
    
    for (int i = 0; i < 256; i++) {
        input_data[i] = (uint8_t)((i * 7 + 13) % 256);
    }
    
    size_t input_len = 100;
    
    printf("Original data (first 20 bytes): ");
    for (size_t i = 0; i < 20 && i < input_len; i++) {
        printf("%02X ", input_data[i]);
    }
    printf("\n");
    
    for (enum compression_type type = COMPRESS_NONE; type < COMPRESS_TYPE_COUNT; type++) {
        int comp_result = -1;
        int decomp_result = -1;
        
        switch (type) {
            case COMPRESS_NONE:
                if (sizeof(compressed) >= input_len) {
                    memcpy(compressed, input_data, input_len);
                    comp_result = (int)input_len;
                    memcpy(decompressed, compressed, input_len);
                    decomp_result = (int)input_len;
                }
                break;
                
            case COMPRESS_RLE:
                comp_result = compress_rle(input_data, input_len, compressed, sizeof(compressed));
                if (comp_result > 0) {
                    decomp_result = decompress_rle(compressed, (size_t)comp_result, decompressed, sizeof(decompressed));
                }
                break;
                
            case COMPRESS_DELTA:
                comp_result = compress_delta(input_data, input_len, compressed, sizeof(compressed));
                if (comp_result > 0) {
                    decomp_result = decompress_delta(compressed, (size_t)comp