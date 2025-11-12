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
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_len - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            output[out_idx++] = current;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            output[out_idx++] = current;
            in_idx++;
        }
    }
    
    return out_idx;
}

size_t compress_lzw_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t copy_len = (input_len < output_len) ? input_len : output_len;
    memcpy(output, input, copy_len);
    return copy_len;
}

size_t compress_huffman_stub(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len) {
    if (input == NULL || output == NULL || input_len == 0 || output_len == 0) return 0;
    
    size_t copy_len = (input_len < output_len) ? input_len : output_len;
    memcpy(output, input, copy_len);
    return copy_len;
}

struct compression_result compress_data(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, enum compression_type type) {
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
    char input_str[1024];
    int choice;
    
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
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(input_buffer, input_str, input_len);
    
    print_compression_menu();
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
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
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    struct compression_result result = compress_data(input_buffer, input_len, output_buffer, sizeof(output_buffer), comp_type);
    
    printf("\nCompression Results