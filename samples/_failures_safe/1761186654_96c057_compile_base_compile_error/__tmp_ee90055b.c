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
        
        if (count > 1) {
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            output[out_idx++] = current;
            output[out_idx++] = 1;
        }
        
        i += count;
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
    
    size_t copy_len = input_len < output_len ? input_len : output_len;
    memcpy(output, input, copy_len);
    return copy_len;
}

struct compression_result compress_data(const uint8_t *input, size_t input_len, enum compression_type type) {
    struct compression_result result = {0, 0, COMP_NONE};
    
    if (input == NULL || input_len == 0) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(input_len * 2);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t compressed_size = 0;
    
    switch (type) {
        case COMP_RLE:
            compressed_size = compress_rle(input, input_len, output_buffer, input_len * 2);
            break;
        case COMP_LZW:
            compressed_size = compress_lzw_stub(input, input_len, output_buffer, input_len * 2);
            break;
        case COMP_HUFFMAN:
            compressed_size = compress_huffman_stub(input, input_len, output_buffer, input_len * 2);
            break;
        case COMP_NONE:
        default:
            compressed_size = 0;
            break;
    }
    
    free(output_buffer);
    
    result.original_size = input_len;
    result.compressed_size = compressed_size;
    result.type = type;
    
    return result;
}

void print_compression_menu(void) {
    printf("Available compression types:\n");
    printf("1. None\n");
    printf("2. Run-Length Encoding (RLE)\n");
    printf("3. LZW (stub)\n");
    printf("4. Huffman (stub)\n");
    printf("Enter choice (1-4): ");
}

int main(void) {
    uint8_t input_data[256];
    size_t data_size = 0;
    
    printf("Enter up to 255 characters of data to compress: ");
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && data_size < sizeof(input_data) - 1) {
        if (isprint(c)) {
            input_data[data_size++] = (uint8_t)c;
        }
    }
    input_data[data_size] = '\0';
    
    if (data_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    print_compression_menu();
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum compression_type comp_type;
    switch (choice) {
        case 1: comp_type = COMP_NONE; break;
        case 2: comp_type = COMP_RLE; break;
        case 3: comp_type = COMP_LZW; break;
        case 4: comp_type = COMP_HUFFMAN; break;
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    struct compression_result result = compress_data(input_data, data_size, comp_type);
    
    printf("\nCompression Results:\n");
    printf("Original size: %zu bytes\n", result.original_size);
    printf("Compressed size: %zu bytes\n", result.compressed_size);
    
    if (result.compressed_size > 0) {
        double ratio = (double)result.original_size / result.compressed_size;
        printf("Compression ratio: %.2f