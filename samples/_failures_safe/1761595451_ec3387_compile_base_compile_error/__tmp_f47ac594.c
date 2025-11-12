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
        
        if (count >= 3) {
            output[out_idx++] = current;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            output[out_idx++] = current;
            i++;
        }
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_len) {
        if (i + 2 < input_len && input[i] == input[i + 1]) {
            uint8_t value = input[i];
            uint8_t count = input[i + 2];
            
            if (count < 3) return 0;
            
            for (uint8_t j = 0; j < count && out_idx < output_len; j++) {
                output[out_idx++] = value;
            }
            i += 3;
        } else {
            output[out_idx++] = input[i++];
        }
    }
    
    return out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *data, size_t data_len, uint8_t *output, size_t output_len) {
    struct compression_result result = {0, 0, COMP_NONE};
    if (data == NULL || output == NULL || data_len == 0 || output_len == 0) return result;
    
    result.original_size = data_len;
    result.type = type;
    
    switch (type) {
        case COMP_RLE:
            result.compressed_size = compress_rle(data, data_len, output, output_len);
            break;
        case COMP_LZW:
        case COMP_HUFFMAN:
            memcpy(output, data, data_len > output_len ? output_len : data_len);
            result.compressed_size = data_len > output_len ? output_len : data_len;
            break;
        case COMP_NONE:
        default:
            memcpy(output, data, data_len > output_len ? output_len : data_len);
            result.compressed_size = data_len > output_len ? output_len : data_len;
            break;
    }
    
    return result;
}

int main(void) {
    uint8_t test_data[] = {1, 1, 1, 1, 2, 2, 2, 3, 4, 4, 4, 4, 4, 5, 6, 6, 6};
    size_t data_len = sizeof(test_data);
    uint8_t compressed[256];
    uint8_t decompressed[256];
    
    printf("Original data (%zu bytes): ", data_len);
    for (size_t i = 0; i < data_len; i++) {
        printf("%d ", test_data[i]);
    }
    printf("\n");
    
    enum compression_type algorithms[] = {COMP_NONE, COMP_RLE, COMP_LZW, COMP_HUFFMAN};
    const char *algo_names[] = {"None", "RLE", "LZW", "Huffman"};
    
    for (int i = 0; i < 4; i++) {
        struct compression_result result = perform_compression(algorithms[i], test_data, data_len, compressed, sizeof(compressed));
        
        printf("\n%s Compression:\n", algo_names[i]);
        printf("Original size: %zu bytes\n", result.original_size);
        printf("Compressed size: %zu bytes\n", result.compressed_size);
        
        if (result.compressed_size > 0) {
            printf("Compression ratio: %.2f%%\n", 
                   (1.0 - (double)result.compressed_size / result.original_size) * 100.0);
            
            printf("Compressed data: ");
            for (size_t j = 0; j < result.compressed_size && j < 20; j++) {
                printf("%d ", compressed[j]);
            }
            if (result.compressed_size > 20) printf("...");
            printf("\n");
            
            if (algorithms[i] == COMP_RLE) {
                size_t decomp_size = decompress_rle(compressed, result.compressed_size, decompressed, sizeof(decompressed));
                if (decomp_size == data_len && memcmp(test_data