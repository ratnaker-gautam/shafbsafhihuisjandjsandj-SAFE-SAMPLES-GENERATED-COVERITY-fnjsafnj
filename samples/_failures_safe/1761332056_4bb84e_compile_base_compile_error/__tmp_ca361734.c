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
            if (out_idx + 2 >= output_len) break;
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

size_t compress_lzw_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t bytes_to_copy = input_len < output_len ? input_len : output_len;
    memcpy(output, input, bytes_to_copy);
    return bytes_to_copy;
}

size_t compress_huffman_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t bytes_to_copy = input_len < output_len ? input_len : output_len;
    memcpy(output, input, bytes_to_copy);
    return bytes_to_copy;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
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
            result.compressed_size = 0;
            break;
    }
    
    return result;
}

int main(void) {
    uint8_t input_data[256];
    uint8_t output_data[512];
    
    printf("Enter text to compress (max 255 characters): ");
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    printf("Available compression methods:\n");
    printf("1. Run-Length Encoding (RLE)\n");
    printf("2. LZW (stub)\n");
    printf("3. Huffman (stub)\n");
    printf("Select method (1-3): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum compression_type comp_type;
    switch (choice) {
        case 1:
            comp_type = COMP_RLE;
            break;
        case 2:
            comp_type = COMP_LZW;
            break;
        case 3:
            comp_type = COMP_HUFFMAN;
            break;
        default:
            fprintf(stderr, "Invalid selection\n");
            return 1;
    }
    
    struct compression_result result = perform_compression(comp_type, input_data, input_len, output_data, sizeof(output_data));
    
    if (result.compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", result.original_size);
    printf("Compressed size: %zu bytes\n", result.compressed_size);
    
    double ratio = (double)result.compressed_size / result.original_size * 100.0;
    printf("Compression