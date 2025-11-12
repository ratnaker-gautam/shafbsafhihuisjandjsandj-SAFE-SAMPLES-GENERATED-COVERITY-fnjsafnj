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
    size_t dict_size = 256;
    
    for (size_t i = 0; i < input_len && out_idx < input_len; i++) {
        if (out_idx + 1 > input_len) return 0;
        output[out_idx++] = input[i];
        
        if (i + 1 < input_len && input[i] == input[i + 1]) {
            if (out_idx + 1 > input_len) return 0;
            output[out_idx++] = 1;
            i++;
        }
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input_len == 0 || output == NULL) return 0;
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input_len; i++) {
        freq[input[i]]++;
    }
    
    size_t out_idx = 0;
    for (int i = 0; i < 256 && out_idx < input_len; i++) {
        if (freq[i] > 0) {
            if (out_idx + 2 > input_len) return 0;
            output[out_idx++] = (uint8_t)i;
            output[out_idx++] = freq[i];
        }
    }
    
    return out_idx;
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
            if (output_len >= input_len) {
                compressed_size = compress_none(input, input_len, output);
            }
            break;
            
        case COMP_RLE:
            if (output_len >= input_len * 2) {
                compressed_size = compress_rle(input, input_len, output);
            }
            break;
            
        case COMP_LZW:
            if (output_len >= input_len) {
                compressed_size = compress_lzw(input, input_len, output);
            }
            break;
            
        case COMP_HUFFMAN:
            if (output_len >= 512) {
                compressed_size = compress_huffman(input, input_len, output);
            }
            break;
            
        default:
            break;
    }
    
    result.compressed_size = compressed_size;
    return result;
}

void print_hex(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len && i < 32; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (len > 0) printf("\n");
}

int main(void) {
    uint8_t input_buffer[256];
    uint8_t output_buffer[512];
    
    printf("Enter text to compress (max 255 characters): ");
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nAvailable compression