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
        
        if (count >= 3 || current == 0xFF) {
            if (out_idx + 2 >= output_len) break;
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && out_idx < output_len; j++) {
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_len) {
        if (i + 2 < input_len && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count > output_len) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            
            i += 3;
        } else {
            output[out_idx++] = input[i++];
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
        case COMP_HUFFMAN:
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                result.compressed_size = input_len;
            }
            break;
            
        default:
            break;
    }
    
    return result;
}

int main(void) {
    uint8_t test_data[] = {0x01, 0x01, 0x01, 0x02, 0x02, 0x03, 0x04, 0x04, 0x04, 0x04};
    size_t data_size = sizeof(test_data);
    uint8_t compressed[256] = {0};
    uint8_t decompressed[256] = {0};
    
    printf("Original data: ");
    for (size_t i = 0; i < data_size; i++) {
        printf("%02X ", test_data[i]);
    }
    printf("\n");
    
    enum compression_type algorithms[] = {COMP_NONE, COMP_RLE, COMP_LZW, COMP_HUFFMAN};
    const char *algorithm_names[] = {"None", "RLE", "LZW", "Huffman"};
    
    for (int i = 0; i < 4; i++) {
        struct compression_result result = perform_compression(algorithms[i], test_data, data_size, compressed, sizeof(compressed));
        
        printf("\nAlgorithm: %s\n", algorithm_names[i]);
        printf("Original size: %zu\n", result.original_size);
        printf("Compressed size: %zu\n", result.compressed_size);
        
        if (result.compressed_size > 0) {
            printf("Compressed data: ");
            for (size_t j = 0; j < result.compressed_size; j++) {
                printf("%02X ", compressed[j]);
            }
            printf("\n");
            
            if (algorithms[i] == COMP_RLE) {
                size_t decomp_size = decompress_rle(compressed, result.compressed_size, decompressed, sizeof(decompressed));
                if (decomp_size == data_size && memcmp(test_data, decompressed, data_size) == 0) {
                    printf("Decompression successful\n");
                } else {
                    printf