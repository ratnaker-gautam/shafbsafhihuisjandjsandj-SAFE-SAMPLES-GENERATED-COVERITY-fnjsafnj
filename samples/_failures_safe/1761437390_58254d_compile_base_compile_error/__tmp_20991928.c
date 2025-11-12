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
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (out_idx + 3 > output_len) break;
            output[out_idx++] = current;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            output[out_idx++] = current;
            in_idx++;
        }
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t dict_size = 256;
    
    for (size_t i = 0; i < input_len && out_idx < output_len; i++) {
        if (out_idx + 2 > output_len) break;
        
        if (i + 1 < input_len && input[i] == input[i + 1]) {
            uint16_t code = (uint16_t)(dict_size++);
            if (out_idx + 2 <= output_len) {
                output[out_idx++] = (uint8_t)(code >> 8);
                output[out_idx++] = (uint8_t)(code & 0xFF);
                i++;
            }
        } else {
            output[out_idx++] = input[i];
        }
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input_len; i++) {
        freq[input[i]]++;
    }
    
    size_t out_idx = 0;
    for (int i = 0; i < 256 && out_idx < output_len; i++) {
        if (freq[i] > 0) {
            output[out_idx++] = (uint8_t)i;
            if (out_idx < output_len) {
                output[out_idx++] = freq[i];
            }
        }
    }
    
    if (out_idx + input_len <= output_len) {
        memcpy(output + out_idx, input, input_len);
        out_idx += input_len;
    }
    
    return out_idx;
}

int compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, enum compression_type type, struct compression_result *result) {
    if (input == NULL || output == NULL || result == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t compressed_size = 0;
    
    switch (type) {
        case COMP_RLE:
            compressed_size = compress_rle(input, input_len, output, output_len);
            break;
        case COMP_LZW:
            compressed_size = compress_lzw(input, input_len, output, output_len);
            break;
        case COMP_HUFFMAN:
            compressed_size = compress_huffman(input, input_len, output, output_len);
            break;
        case COMP_NONE:
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                compressed_size = input_len;
            }
            break;
        default:
            return 0;
    }
    
    if (compressed_size == 0 || compressed_size > output_len) return 0;
    
    result->original_size = input_len;
    result->compressed_size = compressed_size;
    result->type = type;
    
    return 1;
}

void print_compression_menu(void) {
    printf("Compression types:\n");
    printf("1. No compression\n");
    printf("2. Run-Length Encoding\n");
    printf("3. LZW compression\n");
    printf("4. Huffman coding\n");
    printf("Enter choice (1-4): ");
}

int main(void) {
    uint8_t input_buffer[1024];
    uint8_t output_buffer[2048];
    struct compression_result result;
    
    printf("Enter text to compress (max 1023 characters): ");
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)