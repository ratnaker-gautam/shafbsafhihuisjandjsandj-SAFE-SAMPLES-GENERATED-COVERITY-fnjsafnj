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
        
        if (count > 1) {
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            output[out_idx++] = current;
            output[out_idx++] = 1;
        }
        
        i += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t dict_size = 256;
    
    for (size_t i = 0; i < input_len && out_idx < output_len; i++) {
        if (input[i] < dict_size) {
            output[out_idx++] = input[i];
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
    
    return out_idx;
}

struct compression_result perform_compression(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, enum compression_type type) {
    struct compression_result result = {0, 0, COMP_NONE};
    
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return result;
    }
    
    result.original_size = input_len;
    result.type = type;
    
    switch (type) {
        case COMP_RLE:
            result.compressed_size = compress_rle(input, input_len, output, output_len);
            break;
        case COMP_LZW:
            result.compressed_size = compress_lzw(input, input_len, output, output_len);
            break;
        case COMP_HUFFMAN:
            result.compressed_size = compress_huffman(input, input_len, output, output_len);
            break;
        case COMP_NONE:
        default:
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                result.compressed_size = input_len;
            }
            break;
    }
    
    return result;
}

void print_compression_type(enum compression_type type) {
    switch (type) {
        case COMP_NONE:
            printf("No compression");
            break;
        case COMP_RLE:
            printf("Run-length encoding");
            break;
        case COMP_LZW:
            printf("Lempel-Ziv-Welch");
            break;
        case COMP_HUFFMAN:
            printf("Huffman coding");
            break;
        default:
            printf("Unknown");
            break;
    }
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    size_t input_len = 0;
    
    printf("Enter text to compress (max 255 characters): ");
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nAvailable compression methods:\n");
    printf("1. No compression\n");
    printf("2. Run-length encoding\n");
    printf("3. Lempel-Ziv-Welch\n");
    printf("4. Huffman coding\n");
    
    printf("Select method (1-4): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("