//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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

void compress_rle(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL || input_len == 0) {
        *output_len = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < *output_len - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > *output_len) break;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    *output_len = out_idx;
}

void compress_lzw_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL || input_len == 0) {
        *output_len = 0;
        return;
    }
    
    size_t copy_len = input_len < *output_len ? input_len : *output_len;
    memcpy(output, input, copy_len);
    *output_len = copy_len;
}

void compress_huffman_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL || input_len == 0) {
        *output_len = 0;
        return;
    }
    
    for (size_t i = 0; i < input_len && i < *output_len; i++) {
        output[i] = input[i] ^ 0x55;
    }
    *output_len = input_len < *output_len ? input_len : *output_len;
}

struct compression_result compress_data(const uint8_t *input, size_t input_len, enum compression_type type) {
    struct compression_result result = {0, 0, type};
    
    if (input == NULL || input_len == 0) {
        return result;
    }
    
    result.original_size = input_len;
    
    uint8_t *output_buffer = malloc(input_len * 2 + 100);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t output_len = input_len * 2 + 100;
    
    switch (type) {
        case COMPRESS_NONE:
            if (output_len > input_len) {
                memcpy(output_buffer, input, input_len);
                result.compressed_size = input_len;
            }
            break;
            
        case COMPRESS_RLE:
            compress_rle(input, input_len, output_buffer, &output_len);
            result.compressed_size = output_len;
            break;
            
        case COMPRESS_LZW:
            compress_lzw_stub(input, input_len, output_buffer, &output_len);
            result.compressed_size = output_len;
            break;
            
        case COMPRESS_HUFFMAN:
            compress_huffman_stub(input, input_len, output_buffer, &output_len);
            result.compressed_size = output_len;
            break;
            
        default:
            result.compressed_size = 0;
            break;
    }
    
    free(output_buffer);
    return result;
}

void print_compression_menu(void) {
    printf("Available compression methods:\n");
    printf("1. No compression\n");
    printf("2. Run-Length Encoding (RLE)\n");
    printf("3. LZW compression\n");
    printf("4. Huffman coding\n");
    printf("Enter choice (1-4): ");
}

int main(void) {
    uint8_t input_data[1024];
    size_t data_size = 0;
    
    printf("Enter data to compress (max 1023 characters): ");
    
    int c;
    while (data_size < sizeof(input_data) - 1 && (c = getchar()) != '\n' && c != EOF) {
        input_data[data_size++] = (uint8_t)c;
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
        case 1: comp_type = COMPRESS_NONE; break;
        case 2: comp_type = COMPRESS_RLE; break;
        case 3: comp_type = COMPRESS_LZW; break;
        case 4: comp_type = COMPRESS_HUFFMAN; break;
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    struct compression_result result = compress_data