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
        
        if (i > 0 && input[i] == input[i-1] && dict_size < 65536) {
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
    
    return out_idx;
}

struct compression_result compress_data(const uint8_t *input, size_t input_len, enum compression_type type) {
    struct compression_result result = {0, 0, COMP_NONE};
    
    if (input == NULL || input_len == 0) return result;
    
    size_t max_output_len = input_len * 2 + 256;
    if (max_output_len < input_len) return result;
    
    uint8_t *output_buffer = malloc(max_output_len);
    if (output_buffer == NULL) return result;
    
    size_t compressed_size = 0;
    
    switch (type) {
        case COMP_RLE:
            compressed_size = compress_rle(input, input_len, output_buffer, max_output_len);
            break;
        case COMP_LZW:
            compressed_size = compress_lzw(input, input_len, output_buffer, max_output_len);
            break;
        case COMP_HUFFMAN:
            compressed_size = compress_huffman(input, input_len, output_buffer, max_output_len);
            break;
        case COMP_NONE:
        default:
            compressed_size = 0;
            break;
    }
    
    result.original_size = input_len;
    result.compressed_size = compressed_size;
    result.type = type;
    
    free(output_buffer);
    return result;
}

void print_compression_menu(void) {
    printf("Available compression methods:\n");
    printf("1. Run-Length Encoding (RLE)\n");
    printf("2. LZW Compression\n");
    printf("3. Huffman Coding\n");
    printf("Enter choice (1-3): ");
}

int main(void) {
    uint8_t input_buffer[1024];
    size_t input_len = 0;
    
    printf("Enter text to compress (max 1023 characters): ");
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    print_compression_menu();
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum compression_type comp_type;
    switch (choice) {
        case 1:
            comp_type = COMP_RLE;
            break;
        case 2: