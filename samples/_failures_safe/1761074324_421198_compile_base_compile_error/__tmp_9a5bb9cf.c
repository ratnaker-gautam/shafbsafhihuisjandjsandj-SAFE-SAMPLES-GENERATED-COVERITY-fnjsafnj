//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_LZW,
    COMPRESS_HUFFMAN
};

struct compression_result {
    size_t original_size;
    size_t compressed_size;
    enum compression_type type;
};

size_t compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 <= output_len) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len) {
        output[out_idx++] = input[in_idx++];
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len) {
        output[out_idx++] = input[in_idx++];
    }
    
    return out_idx;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    struct compression_result result = {0, 0, COMPRESS_NONE};
    
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return result;
    }
    
    result.original_size = input_len;
    result.type = type;
    
    switch (type) {
        case COMPRESS_RLE:
            result.compressed_size = compress_rle(input, input_len, output, output_len);
            break;
        case COMPRESS_LZW:
            result.compressed_size = compress_lzw(input, input_len, output, output_len);
            break;
        case COMPRESS_HUFFMAN:
            result.compressed_size = compress_huffman(input, input_len, output, output_len);
            break;
        case COMPRESS_NONE:
        default:
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                result.compressed_size = input_len;
            }
            break;
    }
    
    return result;
}

void print_compression_info(const struct compression_result *result) {
    if (result == NULL || result->original_size == 0) {
        return;
    }
    
    const char *type_name = "Unknown";
    switch (result->type) {
        case COMPRESS_NONE: type_name = "None"; break;
        case COMPRESS_RLE: type_name = "RLE"; break;
        case COMPRESS_LZW: type_name = "LZW"; break;
        case COMPRESS_HUFFMAN: type_name = "Huffman"; break;
    }
    
    printf("Compression type: %s\n", type_name);
    printf("Original size: %zu bytes\n", result->original_size);
    printf("Compressed size: %zu bytes\n", result->compressed_size);
    
    if (result->original_size > 0) {
        double ratio = (double)result->compressed_size / result->original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    
    for (size_t i = 0; i < sizeof(input_data); i++) {
        input_data[i] = (uint8_t)((i * 7 + 13) % 256);
    }
    
    printf("Available compression methods:\n");
    printf("1. None\n");
    printf("2. RLE\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Select method (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 4) {
        printf("Invalid choice\n");
        return 1;
    }
    
    enum compression_type selected_type;
    switch (choice) {
        case 1: