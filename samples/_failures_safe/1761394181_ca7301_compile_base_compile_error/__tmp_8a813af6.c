//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct compression_params {
    enum compression_type type;
    uint32_t block_size;
    uint8_t level;
};

int validate_params(const struct compression_params *params) {
    if (params == NULL) return 0;
    if (params->type > COMP_HUFFMAN) return 0;
    if (params->block_size == 0 || params->block_size > 1048576) return 0;
    if (params->level > 9) return 0;
    return 1;
}

size_t compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < input_len) return 0;
    memcpy(output, input, input_len);
    return input_len;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 1) {
        uint8_t current = input[in_idx];
        uint8_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_len) break;
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < 4) return 0;
    
    output[0] = (uint8_t)(input_len >> 24);
    output[1] = (uint8_t)(input_len >> 16);
    output[2] = (uint8_t)(input_len >> 8);
    output[3] = (uint8_t)input_len;
    
    size_t copy_len = input_len;
    if (copy_len > output_len - 4) copy_len = output_len - 4;
    
    memcpy(output + 4, input, copy_len);
    return copy_len + 4;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < 5) return 0;
    
    output[0] = 0x48;
    output[1] = 0x55;
    output[2] = 0x46;
    output[3] = 0x46;
    
    size_t copy_len = input_len;
    if (copy_len > output_len - 5) copy_len = output_len - 5;
    
    memcpy(output + 4, input, copy_len);
    output[4 + copy_len] = 0xFF;
    return copy_len + 5;
}

size_t compress_data(const struct compression_params *params, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (!validate_params(params) || input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len == 0) return 0;
    
    size_t result = 0;
    
    switch (params->type) {
        case COMP_NONE:
            result = compress_none(input, input_len, output, output_len);
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
    struct compression_params params;
    uint8_t input_buffer[256];
    uint8_t output_buffer[512];
    size_t input_len = 0;
    
    printf("Enter compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    int type_input;
    if (scanf("%d", &type_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    params.type = (enum compression_type)type_input;
    
    printf("Enter block size (1-1048576): ");
    if (scanf("%u", &params.block_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter compression level (0-9): ");
    if (scanf("%hhu", &params.level) != 1) {