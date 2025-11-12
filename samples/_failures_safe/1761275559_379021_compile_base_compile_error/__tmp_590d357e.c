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
            uint16_t combined = (input[i] << 8) | input[i + 1];
            output[out_idx++] = (combined >> 8) & 0xFF;
            output[out_idx++] = combined & 0xFF;
        } else {
            output[out_idx++] = input[i];
            output[out_idx++] = 0;
        }
    }
    
    *output_len = out_idx;
}

static void compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = 0;
        return;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (out_idx >= *output_len) {
            *output_len = 0;
            return;
        }
        output[out_idx++] = input[i] ^ 0x55;
    }
    
    *output_len = out_idx;
}

static struct compression_result compress_data(const uint8_t *input, size_t input_len, enum compression_type type) {
    struct compression_result result = {0};
    result.original_size = input_len;
    result.type = type;
    
    if (input == NULL || input_len == 0 || input_len > 1024) {
        result.success = 0;
        return result;
    }
    
    size_t output_len = input_len * 2 + 16;
    uint8_t *output = malloc(output_len);
    if (output == NULL) {
        result.success = 0;
        return result;
    }
    
    switch (type) {
        case COMP_NONE:
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                result.compressed_size = input_len;
                result.success = 1;
            } else {
                result.success = 0;
            }
            break;
            
        case COMP_RLE:
            compress_rle(input, input_len, output, &output_len);
            if (output_len > 0 && output_len < input_len) {
                result.compressed_size = output_len;
                result.success = 1;
            } else {
                result.success = 0;
            }
            break;
            
        case COMP_LZW:
            compress_lzw(input, input_len, output, &output_len);
            if (output_len > 0) {
                result.compressed_size = output_len;
                result.success = 1;
            } else {
                result.success = 0;
            }
            break;
            
        case COMP_HUFFMAN:
            compress_huffman(input, input_len, output, &output_len);
            if (output_len > 0) {
                result.compressed_size = output_len;
                result.success = 1;
            } else {
                result.success = 0;
            }
            break;
            
        default:
            result.success = 0;
            break;
    }
    
    if (result.success) {
        printf("Compression successful: %zu -> %zu bytes (", 
               result.original_size, result.compressed_size);
        switch (type) {
            case COMP_NONE: printf("none"); break;
            case COMP_RLE: printf("RLE"); break;
            case COMP_LZW: printf("LZW"); break;
            case COMP_HUFFMAN: printf("Huffman"); break;
        }
        printf(")\n");
    } else {
        printf("Compression failed\n");
    }
    
    free(output);
    return result;
}

int main(void) {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42,