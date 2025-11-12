//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct compression_params {
    enum compression_type type;
    uint32_t max_size;
    uint8_t quality;
};

static int validate_params(const struct compression_params *params) {
    if (params == NULL) return 0;
    if (params->type > COMP_HUFFMAN) return 0;
    if (params->max_size == 0 || params->max_size > 1000000) return 0;
    if (params->quality > 100) return 0;
    return 1;
}

static size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= output_len) break;
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return out_idx;
}

static size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len) {
        if (in_idx + 1 < input_len) {
            uint16_t code = (uint16_t)((input[in_idx] << 8) | input[in_idx + 1]);
            if (out_idx + 2 <= output_len) {
                output[out_idx++] = (uint8_t)(code >> 8);
                output[out_idx++] = (uint8_t)(code & 0xFF);
                in_idx += 2;
            } else {
                break;
            }
        } else {
            if (out_idx < output_len) {
                output[out_idx++] = input[in_idx++];
            } else {
                break;
            }
        }
    }
    
    return out_idx;
}

static size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len) {
        uint8_t val = input[in_idx++];
        if (out_idx < output_len) {
            output[out_idx++] = val ^ 0x55;
        } else {
            break;
        }
    }
    
    return out_idx;
}

static size_t compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, const struct compression_params *params) {
    if (!validate_params(params) || input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return 0;
    }
    
    if (input_len > params->max_size) {
        return 0;
    }
    
    size_t result = 0;
    
    switch (params->type) {
        case COMP_NONE:
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                result = input_len;
            }
            break;
            
        case COMP_RLE:
            result = compress_rle(input, input_len, output, output_len);
            break;
            
        case COMP_LZW:
            result = compress_lzw(input, input_len, output, output_len);
            break;
            
        case COMP_HUFFMAN:
            result = compress_huffman(input, input_len, output, output_len);
            break;
            
        default:
            result = 0;
            break;
    }
    
    return result;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    struct compression_params params;
    
    printf("Enter compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    int type_input;
    if (scanf("%d", &type_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (type_input < 0 || type_input > 3) {
        printf("Invalid compression type\n");
        return 1;
    }
    params.type = (enum compression_type)type_input;
    
    printf("Enter maximum input size (100-1000000): ");
    if (scanf("%u", &params.max_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter quality level (0-100):