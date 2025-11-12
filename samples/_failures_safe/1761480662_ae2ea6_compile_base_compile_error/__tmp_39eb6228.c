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
    COMP_HUFFMAN,
    COMP_COUNT
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
        if (out_idx >= output_len) break;
        output[out_idx++] = input[i];
        
        if (i > 0 && input[i] == input[i-1] && out_idx < output_len) {
            output[out_idx++] = 2;
            i++;
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
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                result.compressed_size = input_len;
            }
            break;
        default:
            result.compressed_size = 0;
            break;
    }
    
    return result;
}

void print_compression_stats(const struct compression_result *result) {
    if (result == NULL) return;
    
    const char *type_names[] = {"None", "RLE", "LZW", "Huffman"};
    if (result->type < COMP_COUNT) {
        printf("Compression type: %s\n", type_names[result->type]);
    } else {
        printf("Compression type: Unknown\n");
    }
    printf("Original size: %zu bytes\n", result->original_size);
    printf("Compressed size: %zu bytes\n", result->compressed_size);
    
    if (result->original_size > 0 && result->compressed_size > 0) {
        double ratio = (double)result->compressed_size / result->original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    
    const char *test_string = "AAAABBBCCDAAEEFFGGGHHHHHIIIIJJKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ";
    size_t input_len = strlen(test_string);
    
    if (input_len >= sizeof(input_data)) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(input_data, test_string, input_len);
    
    printf("Testing compression algorithms on %zu bytes