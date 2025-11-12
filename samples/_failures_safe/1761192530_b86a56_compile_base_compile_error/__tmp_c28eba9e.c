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
};

size_t compress_none(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len > 0 && output != NULL) {
        memcpy(output, input, input_len);
    }
    return input_len;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len == 0 || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > input_len * 2) return 0;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len == 0 || output == NULL) return 0;
    
    size_t out_idx = 0;
    uint16_t dict[4096];
    uint8_t dict_len = 0;
    
    for (int i = 0; i < 256; i++) {
        dict[i] = (uint16_t)i;
    }
    dict_len = 256;
    
    size_t i = 0;
    while (i < input_len) {
        if (out_idx + 2 > input_len) return 0;
        
        uint16_t code = (uint16_t)input[i];
        output[out_idx++] = (uint8_t)(code >> 8);
        output[out_idx++] = (uint8_t)(code & 0xFF);
        i++;
        
        if (dict_len < 4096 && i < input_len) {
            dict[dict_len++] = (uint16_t)input[i];
        }
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len == 0 || output == NULL) return 0;
    
    uint32_t freq[256] = {0};
    for (size_t i = 0; i < input_len; i++) {
        freq[input[i]]++;
    }
    
    size_t out_idx = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            if (out_idx + 2 > input_len * 3) return 0;
            output[out_idx++] = (uint8_t)i;
            output[out_idx++] = (uint8_t)(freq[i] >> 8);
            output[out_idx++] = (uint8_t)(freq[i] & 0xFF);
        }
    }
    
    if (out_idx + input_len > input_len * 4) return 0;
    memcpy(output + out_idx, input, input_len);
    return out_idx + input_len;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    struct compression_result result = {0, 0, COMP_NONE};
    
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return result;
    }
    
    result.original_size = input_len;
    result.type = type;
    
    size_t compressed_size = 0;
    
    switch (type) {
        case COMP_NONE:
            compressed_size = compress_none(input, input_len, output);
            break;
        case COMP_RLE:
            compressed_size = compress_rle(input, input_len, output);
            break;
        case COMP_LZW:
            compressed_size = compress_lzw(input, input_len, output);
            break;
        case COMP_HUFFMAN:
            compressed_size = compress_huffman(input, input_len, output);
            break;
        default:
            compressed_size = 0;
            break;
    }
    
    if (compressed_size > output_len || compressed_size == 0) {
        result.compressed_size = 0;
    } else {
        result.compressed_size = compressed_size;
    }
    
    return result;
}

int main() {
    uint8_t input_data[256];
    uint8_t output_data[1024];
    
    printf("Enter text to compress (max 255 characters): ");
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n