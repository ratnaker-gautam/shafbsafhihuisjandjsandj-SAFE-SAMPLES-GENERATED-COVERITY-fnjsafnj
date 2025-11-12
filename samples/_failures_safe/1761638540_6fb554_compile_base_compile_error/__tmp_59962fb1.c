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
            if (input_len <= output_len) {
                memcpy(output, input, input_len);
                result.compressed_size = input_len;
            }
            break;
    }
    
    return result;
}

void print_hex(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len && i < 32; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (len > 0) printf("\n");
}

int main(void) {
    uint8_t input_buffer[256];
    uint8_t output_buffer[512];
    
    printf("Enter text to compress (max 255 characters): ");
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nAvailable compression methods:\n");
    printf("1. None (pass-through)\n");
    printf("2. Run-Length Encoding (RLE)\n");
    printf("3. LZW (stub)\n");
    printf("4. Huffman (stub)\n");
    printf("Select method (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
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
            printf("Invalid selection\n");
            return 1;
    }
    
    struct compression_result result = perform_compression(comp_type, input_buffer, input_len