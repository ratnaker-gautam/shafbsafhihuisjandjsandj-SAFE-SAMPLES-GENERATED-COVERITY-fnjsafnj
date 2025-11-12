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
    if (input == NULL || output == NULL || output_len < 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 1) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
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
    if (input == NULL || output == NULL || output_len == 0) return 0;
    
    size_t bytes_to_copy = input_len < output_len ? input_len : output_len;
    memcpy(output, input, bytes_to_copy);
    return bytes_to_copy;
}

size_t compress_huffman_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || output_len == 0) return 0;
    
    size_t bytes_to_copy = input_len < output_len ? input_len : output_len;
    memcpy(output, input, bytes_to_copy);
    return bytes_to_copy;
}

struct compression_result perform_compression(enum compression_type type, const uint8_t *data, size_t data_len) {
    struct compression_result result = {0};
    result.original_size = data_len;
    result.type = type;
    
    if (data == NULL || data_len == 0) {
        return result;
    }
    
    uint8_t *compressed = malloc(data_len * 2);
    if (compressed == NULL) {
        return result;
    }
    
    size_t compressed_size = 0;
    
    switch (type) {
        case COMPRESS_NONE:
            compressed_size = data_len;
            if (compressed_size <= data_len * 2) {
                memcpy(compressed, data, data_len);
            }
            break;
            
        case COMPRESS_RLE:
            compressed_size = compress_rle(data, data_len, compressed, data_len * 2);
            break;
            
        case COMPRESS_LZW:
            compressed_size = compress_lzw_stub(data, data_len, compressed, data_len * 2);
            break;
            
        case COMPRESS_HUFFMAN:
            compressed_size = compress_huffman_stub(data, data_len, compressed, data_len * 2);
            break;
            
        default:
            compressed_size = 0;
            break;
    }
    
    result.compressed_size = compressed_size;
    
    if (compressed_size > 0 && compressed_size < data_len) {
        printf("Compression successful: %zu -> %zu bytes\n", data_len, compressed_size);
    } else if (compressed_size >= data_len) {
        printf("No compression achieved: %zu -> %zu bytes\n", data_len, compressed_size);
    } else {
        printf("Compression failed\n");
    }
    
    free(compressed);
    return result;
}

void print_compression_types(void) {
    printf("Available compression types:\n");
    printf("0: No compression\n");
    printf("1: Run-Length Encoding (RLE)\n");
    printf("2: LZW (stub)\n");
    printf("3: Huffman (stub)\n");
}

int main(void) {
    char input_buffer[1024];
    uint8_t data[512];
    size_t data_len = 0;
    
    printf("Enter text to compress (max 511 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > 511) {
        input_len = 511;
    }
    
    memcpy(data, input_buffer, input_len);
    data_len = input_len;
    
    print_compression_types();
    printf("Select compression type (0-3): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading selection\n");
        return 1;
    }
    
    int selection = atoi(input_buffer);
    if (selection < 0 || selection > 3) {
        printf("Invalid