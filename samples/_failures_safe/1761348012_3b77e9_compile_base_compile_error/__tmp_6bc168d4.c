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

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_len - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_len) break;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t dict_size = 256;
    
    for (size_t i = 0; i < input_len && out_idx < output_len; i++) {
        if (out_idx + 2 > output_len) break;
        
        output[out_idx++] = input[i];
        
        if (i + 1 < input_len && dict_size < 65536) {
            dict_size++;
        }
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    uint8_t freq[256] = {0};
    
    for (size_t i = 0; i < input_len; i++) {
        freq[input[i]]++;
    }
    
    for (int i = 0; i < 256 && out_idx < output_len; i++) {
        if (freq[i] > 0) {
            output[out_idx++] = (uint8_t)i;
            if (out_idx < output_len) {
                output[out_idx++] = freq[i];
            }
        }
    }
    
    for (size_t i = 0; i < input_len && out_idx < output_len; i++) {
        output[out_idx++] = input[i];
    }
    
    return out_idx;
}

struct compression_result compress_data(const uint8_t *input, size_t input_len, enum compression_type type) {
    struct compression_result result = {0, 0, type};
    
    if (input == NULL || input_len == 0) {
        return result;
    }
    
    result.original_size = input_len;
    size_t max_output_size = input_len * 2 + 256;
    
    if (max_output_size < input_len) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(max_output_size);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t compressed_size = 0;
    
    switch (type) {
        case COMP_RLE:
            compressed_size = compress_rle(input, input_len, output_buffer, max_output_size);
            break;
        case COMP_LZW:
            compressed_size = compress_lzw(input, input_len, output_buffer, max_output_size);
            break;
        case COMP_HUFFMAN:
            compressed_size = compress_huffman(input, input_len, output_buffer, max_output_size);
            break;
        case COMP_NONE:
        default:
            if (input_len <= max_output_size) {
                memcpy(output_buffer, input, input_len);
                compressed_size = input_len;
            }
            break;
    }
    
    result.compressed_size = compressed_size;
    
    if (compressed_size > 0 && compressed_size <= max_output_size) {
        printf("Compressed %zu bytes to %zu bytes using ", input_len, compressed_size);
        switch (type) {
            case COMP_NONE: printf("no compression"); break;
            case COMP_RLE: printf("RLE"); break;
            case COMP_LZW: printf("LZW"); break;
            case COMP_HUFFMAN: printf("Huffman"); break;
        }
        printf("\n");
        
        printf("First 16 bytes of compressed data: ");
        for (size_t i = 0; i < compressed_size && i < 16; i++) {
            printf("%02X ", output_buffer[i]);
        }
        printf("\n");
    } else {
        printf("Compression failed or produced invalid result\n");
    }
    
    free(output_buffer);
    return result;
}

int main(void) {
    uint8_t test_data[] = {
        'A', 'A', 'A', 'A', 'B', 'B', 'C', 'D', 'D', 'D',
        'E', 'F', 'F', 'G', 'H', 'H', 'H', 'H