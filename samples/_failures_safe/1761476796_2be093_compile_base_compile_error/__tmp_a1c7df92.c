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
        
        if (out_idx + 2 > output_len) break;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_len && out_idx < output_len; i++) {
        if (out_idx < output_len) {
            output[out_idx++] = input[i];
        }
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_len && out_idx < output_len; i++) {
        if (out_idx < output_len) {
            output[out_idx++] = input[i] ^ 0x55;
        }
    }
    
    return out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    struct compression_result result = {0};
    result.original_size = input_len;
    result.type = type;
    
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return result;
    }
    
    switch (type) {
        case COMP_NONE:
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                result.compressed_size = input_len;
            }
            break;
            
        case COMP_RLE:
            result.compressed_size = compress_rle(input, input_len, output, output_len);
            break;
            
        case COMP_LZW:
            result.compressed_size = compress_lzw(input, input_len, output, output_len);
            break;
            
        case COMP_HUFFMAN:
            result.compressed_size = compress_huffman(input, input_len, output, output_len);
            break;
            
        default:
            result.compressed_size = 0;
            break;
    }
    
    return result;
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
    
    printf("Available compression methods:\n");
    printf("0: No compression\n");
    printf("1: Run-Length Encoding\n");
    printf("2: LZW\n");
    printf("3: Huffman\n");
    printf("Select method (0-3): ");
    
    int method;
    if (scanf("%d", &method) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (method < 0 || method > 3) {
        printf("Invalid method selection\n");
        return 1;
    }
    
    struct compression_result result = perform_compression((enum compression_type)method, input_data, input_len, output_data, sizeof(output_data));
    
    printf("\nCompression Results:\n");
    printf("Method: ");
    switch (result.type) {
        case COMP_NONE: printf("None"); break;
        case COMP_RLE: printf("RLE"); break;
        case COMP_LZW: printf("LZW"); break;
        case COMP_HUFFMAN: printf("Huffman"); break;
    }
    printf("\n");
    printf("Original size: %zu bytes\n", result.original_size);
    printf("Compressed size: %zu