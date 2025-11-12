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

int rle_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len < 2) return -1;
    
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 1) {
        uint8_t current = input[in_idx];
        uint8_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && 
               input[in_idx + count] == current && out_idx < output_len - 1) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return (out_idx < output_len) ? (int)out_idx : -1;
}

int lzw_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len < 2) return -1;
    
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 1) {
        uint8_t val1 = input[in_idx++];
        uint8_t val2 = (in_idx < input_len) ? input[in_idx++] : 0;
        
        uint16_t combined = (uint16_t)((val1 << 8) | val2);
        
        if (out_idx + 2 <= output_len) {
            output[out_idx++] = (combined >> 8) & 0xFF;
            output[out_idx++] = combined & 0xFF;
        } else {
            break;
        }
    }
    
    return (int)out_idx;
}

int huffman_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len < 2) return -1;
    
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len) {
        uint8_t current = input[in_idx++];
        output[out_idx++] = current ^ 0x55;
    }
    
    return (int)out_idx;
}

int compress_data(enum compression_type type, const uint8_t *input, size_t input_len, 
                  uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return -1;
    
    switch (type) {
        case COMP_NONE:
            if (input_len > output_len) return -1;
            memcpy(output, input, input_len);
            return (int)input_len;
            
        case COMP_RLE:
            return rle_compress(input, input_len, output, output_len);
            
        case COMP_LZW:
            return lzw_compress(input, input_len, output, output_len);
            
        case COMP_HUFFMAN:
            return huffman_compress(input, input_len, output, output_len);
            
        default:
            return -1;
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
    
    int compressed_size = compress_data((enum compression_type)method, input_data, 
                                       input_len, output_data, sizeof(output_data));
    
    if (compressed_size < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %d bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_size / input_len) * 100.0);
    return