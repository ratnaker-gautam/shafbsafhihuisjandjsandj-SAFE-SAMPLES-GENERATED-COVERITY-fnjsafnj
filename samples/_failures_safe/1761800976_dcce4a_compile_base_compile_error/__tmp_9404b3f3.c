//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

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
    if (input_len > SIZE_MAX / 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 <= output_len) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < UINT8_MAX && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_len) break;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t compress_lzw_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t copy_len = input_len < output_len ? input_len : output_len;
    memcpy(output, input, copy_len);
    return copy_len;
}

size_t compress_huffman_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t compressed = input_len / 2;
    if (compressed > output_len) compressed = output_len;
    
    for (size_t i = 0; i < compressed && i < input_len; i++) {
        output[i] = input[i] ^ 0x55;
    }
    
    return compressed;
}

struct compression_result perform_compression(enum compression_type type, 
                                            const uint8_t *input, 
                                            size_t input_len,
                                            uint8_t *output,
                                            size_t output_len) {
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
            result.compressed_size = compress_lzw_stub(input, input_len, output, output_len);
            break;
        case COMP_HUFFMAN:
            result.compressed_size = compress_huffman_stub(input, input_len, output, output_len);
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

void print_compression_stats(const struct compression_result *result) {
    if (result == NULL || result->original_size == 0) return;
    
    double ratio = (double)result->compressed_size / result->original_size * 100.0;
    
    const char *type_name;
    switch (result->type) {
        case COMP_RLE: type_name = "RLE"; break;
        case COMP_LZW: type_name = "LZW"; break;
        case COMP_HUFFMAN: type_name = "Huffman"; break;
        case COMP_NONE: type_name = "None"; break;
        default: type_name = "Unknown"; break;
    }
    
    printf("Compression: %s\n", type_name);
    printf("Original size: %zu bytes\n", result->original_size);
    printf("Compressed size: %zu bytes\n", result->compressed_size);
    printf("Compression ratio: %.1f%%\n", ratio);
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    
    for (int i = 0; i < 256; i++) {
        input_data[i] = (uint8_t)(i % 64 + 32);
    }
    
    printf("Testing compression algorithms on sample data:\n\n");
    
    for (enum compression_type type = COMP_NONE; type <= COMP_HUFFMAN; type++) {
        struct compression_result result = perform_compression(type, input_data, 
                                                             sizeof(input_data),
                                                             output_data,
                                                             sizeof(output_data));
        print_compression_stats(&result);
        printf("\n");
    }
    
    printf("Testing RLE on repeated data:\n");
    uint8_t repeated_data[100];
    for (int i = 0; i < 100; i++) {
        repeated_data[i] = 'A';
    }
    
    struct compression_result rle_result = perform_compression(COMP_RLE,