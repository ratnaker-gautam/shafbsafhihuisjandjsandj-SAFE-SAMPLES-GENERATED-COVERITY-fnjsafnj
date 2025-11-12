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

size_t compress_none(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len > output_len) return 0;
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
        
        if (out_idx + 2 > output_len) return 0;
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 1) {
        uint8_t code = input[in_idx] % 16;
        output[out_idx++] = code;
        in_idx++;
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_len < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len) {
        uint8_t compressed = input[in_idx] >> 1;
        output[out_idx++] = compressed;
        in_idx++;
    }
    
    return out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    struct compression_result result = {0, 0, type};
    result.original_size = input_len;
    
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return result;
    }
    
    size_t compressed_size = 0;
    
    switch (type) {
        case COMP_NONE:
            compressed_size = compress_none(input, input_len, output, output_len);
            break;
        case COMP_RLE:
            compressed_size = compress_rle(input, input_len, output, output_len);
            break;
        case COMP_LZW:
            compressed_size = compress_lzw(input, input_len, output, output_len);
            break;
        case COMP_HUFFMAN:
            compressed_size = compress_huffman(input, input_len, output, output_len);
            break;
        default:
            compressed_size = 0;
            break;
    }
    
    result.compressed_size = compressed_size;
    return result;
}

const char* compression_type_name(enum compression_type type) {
    switch (type) {
        case COMP_NONE: return "None";
        case COMP_RLE: return "RLE";
        case COMP_LZW: return "LZW";
        case COMP_HUFFMAN: return "Huffman";
        default: return "Unknown";
    }
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    
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
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nTesting compression algorithms:\n");
    printf("Original data: %s\n", input_data);
    printf("Original size: %zu bytes\n\n", input_len);
    
    enum compression_type types[] = {COMP_NONE, COMP_RLE, COMP_LZW, COMP_HUFFMAN};
    const int num_types = sizeof(types) / sizeof(types[0]);
    
    for (int i = 0; i < num_types; i++) {