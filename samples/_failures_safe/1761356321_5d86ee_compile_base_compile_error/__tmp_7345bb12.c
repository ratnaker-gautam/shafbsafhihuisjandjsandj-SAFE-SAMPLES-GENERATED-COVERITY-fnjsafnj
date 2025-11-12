//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
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
        
        while (in_idx + count < input_len && count < 255 && 
               input[in_idx + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
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
    
    while (in_idx < input_len && out_idx + 1 <= output_len) {
        size_t chunk_size = (input_len - in_idx > 4) ? 4 : (input_len - in_idx);
        
        if (out_idx + chunk_size + 1 > output_len) {
            break;
        }
        
        output[out_idx++] = (uint8_t)chunk_size;
        for (size_t i = 0; i < chunk_size; i++) {
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 1 <= output_len) {
        uint8_t current = input[in_idx++];
        uint8_t encoded = current ^ 0x55;
        
        output[out_idx++] = encoded;
    }
    
    return out_idx;
}

struct compression_result compress_data(const uint8_t *input, size_t input_len, 
                                       uint8_t *output, size_t output_len, 
                                       enum compression_type type) {
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

void print_compression_menu(void) {
    printf("Available compression methods:\n");
    printf("1. None (copy)\n");
    printf("2. Run-Length Encoding (RLE)\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Enter choice (1-4): ");
}

int main(void) {
    uint8_t input_buffer[1024];
    uint8_t output_buffer[2048];
    char input_str[1024];
    
    printf("Enter text to compress (max 1023 characters): ");
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len >= sizeof(input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(input_buffer, input_str, input_len);
    
    print_compression_menu();
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return