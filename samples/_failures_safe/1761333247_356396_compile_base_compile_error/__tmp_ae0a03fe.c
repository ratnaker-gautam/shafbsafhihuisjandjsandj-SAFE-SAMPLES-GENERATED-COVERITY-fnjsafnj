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
    if (input == NULL || output == NULL || output_len == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_len) {
            return 0;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t compress_lzw(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || output_len == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_len && out_idx < output_len; i++) {
        output[out_idx++] = input[i];
    }
    
    return out_idx;
}

size_t compress_huffman(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || output_len == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_len && out_idx < output_len; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    
    return out_idx;
}

struct compression_result compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, enum compression_type type) {
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
            if (output_len >= input_len) {
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
    printf("3. LZW (stub)\n");
    printf("4. Huffman (stub)\n");
    printf("Enter choice (1-4): ");
}

int main(void) {
    uint8_t input_buffer[1024];
    uint8_t output_buffer[2048];
    char choice_str[16];
    int choice;
    
    printf("Enter data to compress (max 1023 characters): ");
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input data provided\n");
        return 1;
    }
    
    print_compression_menu();
    
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Error reading choice\n");
        return 1;
    }
    
    if (sscanf(choice_str, "%d", &choice) != 1) {
        printf("Invalid choice format\n");
        return 1;
    }
    
    enum compression_type comp_type;
    switch (choice) {
        case 1: comp_type = COMPRESS_NONE; break;
        case 2: comp_type = COMPRESS_RLE; break;
        case 3: comp_type = COMPRESS_LZW; break;
        case 4: comp_type = COMPRESS_HUFFMAN; break;
        default:
            printf("Invalid choice: %d\n", choice);
            return 1;
    }
    
    struct compression_result result = compress_data(input_buffer, input_len, output