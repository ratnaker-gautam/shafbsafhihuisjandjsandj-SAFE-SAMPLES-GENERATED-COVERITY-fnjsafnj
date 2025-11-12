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
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0) return 0;
    memcpy(output, input, input_len);
    return input_len;
}

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0) return 0;
    if (input_len > 1024) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < input_len - 1; ) {
        uint8_t current = input[i];
        uint8_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > input_len) break;
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0) return 0;
    if (input_len > 512) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_len && out_idx < input_len; i += 2) {
        if (i + 1 < input_len) {
            output[out_idx++] = (input[i] & 0xF0) | (input[i + 1] >> 4);
        } else {
            output[out_idx++] = input[i];
        }
    }
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0) return 0;
    if (input_len > 256) return 0;
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input_len; i++) {
        freq[input[i]]++;
    }
    
    size_t out_idx = 0;
    for (int i = 0; i < 256 && out_idx < input_len; i++) {
        if (freq[i] > 0) {
            output[out_idx++] = (uint8_t)i;
            output[out_idx++] = freq[i];
        }
    }
    return out_idx;
}

struct compression_result compress_data(const uint8_t *input, size_t input_len, enum compression_type type) {
    struct compression_result result = {0, 0, COMP_NONE};
    if (input == NULL || input_len == 0 || input_len > 1024) return result;
    
    uint8_t output[1024] = {0};
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
            return result;
    }
    
    if (compressed_size > 0 && compressed_size <= input_len) {
        result.original_size = input_len;
        result.compressed_size = compressed_size;
        result.type = type;
    }
    
    return result;
}

void print_compression_menu(void) {
    printf("Available compression types:\n");
    printf("1. No compression\n");
    printf("2. Run-length encoding (RLE)\n");
    printf("3. LZW compression\n");
    printf("4. Huffman coding\n");
    printf("Enter choice (1-4): ");
}

int main(void) {
    uint8_t input_buffer[1024];
    char line_buffer[2048];
    
    printf("Enter text to compress (max 1024 bytes): ");
    if (fgets(line_buffer, sizeof(line_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(line_buffer);
    if (input_len > 0 && line_buffer[input_len - 1] == '\n') {
        line_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > sizeof(input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(input_buffer, line_buffer, input