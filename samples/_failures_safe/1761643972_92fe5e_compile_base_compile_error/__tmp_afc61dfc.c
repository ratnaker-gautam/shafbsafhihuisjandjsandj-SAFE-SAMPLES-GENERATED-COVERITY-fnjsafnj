//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct compression_result {
    size_t original_size;
    size_t compressed_size;
    enum compression_type type;
    uint8_t success;
};

static void compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > *output_len) {
            *output_len = 0;
            return;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    *output_len = out_idx;
}

static void compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_len; i += 2) {
        if (out_idx + 1 >= *output_len) {
            *output_len = 0;
            return;
        }
        
        if (i + 1 < input_len) {
            output[out_idx++] = (input[i] & 0xF0) | (input[i + 1] >> 4);
        } else {
            output[out_idx++] = input[i];
        }
    }
    
    *output_len = out_idx;
}

static void compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input_len; i++) {
        freq[input[i]]++;
    }
    
    size_t out_idx = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            if (out_idx + 2 >= *output_len) {
                *output_len = 0;
                return;
            }
            output[out_idx++] = (uint8_t)i;
            output[out_idx++] = freq[i];
        }
    }
    
    *output_len = out_idx;
}

static struct compression_result compress_data(const uint8_t *input, size_t input_len, enum compression_type type) {
    struct compression_result result = {0};
    result.original_size = input_len;
    result.type = type;
    
    if (input == NULL || input_len == 0 || input_len > 1024 * 1024) {
        result.success = 0;
        return result;
    }
    
    size_t max_output_len = input_len * 2 + 256;
    uint8_t *output_buffer = malloc(max_output_len);
    if (output_buffer == NULL) {
        result.success = 0;
        return result;
    }
    
    size_t output_len = max_output_len;
    
    switch (type) {
        case COMP_NONE:
            if (input_len <= max_output_len) {
                memcpy(output_buffer, input, input_len);
                output_len = input_len;
                result.success = 1;
            } else {
                result.success = 0;
            }
            break;
            
        case COMP_RLE:
            compress_rle(input, input_len, output_buffer, &output_len);
            result.success = (output_len > 0 && output_len <= max_output_len);
            break;
            
        case COMP_LZW:
            compress_lzw(input, input_len, output_buffer, &output_len);
            result.success = (output_len > 0 && output_len <= max_output_len);
            break;
            
        case COMP_HUFFMAN:
            compress_huffman(input, input_len, output_buffer, &output_len);
            result.success = (output_len > 0 && output_len <= max_output_len);
            break;
            
        default:
            result.success = 0;
            break;
    }
    
    if (result.success) {
        result.compressed_size = output_len;
        
        printf("Compression successful:\n");
        printf("  Type: ");
        switch (type) {
            case COMP_NONE: printf("None"); break;
            case COMP_RLE: printf("RLE"); break;
            case COMP_LZW: printf("LZW"); break;
            case COMP_HUFFMAN: printf("Huffman"); break;
        }
        printf("\n");
        printf("  Original size: %zu bytes\n", result.original_size);
        printf("  Compressed size: %zu bytes\n", result.compressed_size);
        printf("  Ratio: %.2f%%\n", (double)result.compressed_size / result.original_size * 100);
    } else {
        printf("Compression failed for type: ");
        switch (type