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

int compress_rle(const char *input, size_t len, char *output, size_t out_size) {
    if (input == NULL || output == NULL || out_size == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        if (out_idx + 2 >= out_size) return -1;
        
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_size) {
    if (input == NULL || output == NULL || out_size < 2) return -1;
    if (len == 0) {
        output[0] = 0;
        return 1;
    }
    
    output[0] = input[0];
    size_t out_idx = 1;
    
    for (size_t i = 1; i < len && out_idx < out_size; i++) {
        output[out_idx++] = input[i];
    }
    
    return out_idx;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_size) {
    if (input == NULL || output == NULL || out_size < 2) return -1;
    if (len == 0) {
        output[0] = 0;
        return 1;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < out_size; i++) {
        output[out_idx++] = input[i] ^ 0x55;
    }
    
    return out_idx;
}

int main(void) {
    char input_buffer[256];
    char output_buffer[512];
    
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("1. None\n");
    printf("2. RLE\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum compression_type method;
    switch (choice) {
        case 1: method = COMPRESS_NONE; break;
        case 2: method = COMPRESS_RLE; break;
        case 3: method = COMPRESS_LZW; break;
        case 4: method = COMPRESS_HUFFMAN; break;
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    int result_size;
    switch (method) {
        case COMPRESS_NONE:
            if (input_len >= sizeof(output_buffer)) {
                fprintf(stderr, "Output buffer too small\n");
                return 1;
            }
            memcpy(output_buffer, input_buffer, input_len);
            result_size = input_len;
            break;
            
        case COMPRESS_RLE:
            result_size = compress_rle(input_buffer, input_len, output_buffer, sizeof(output_buffer));
            if (result_size < 0) {
                fprintf(stderr, "RLE compression failed\n");
                return 1;
            }
            break;
            
        case COMPRESS_LZW:
            result_size = compress_lzw(input_buffer, input_len, output_buffer, sizeof(output_buffer));
            if (result_size < 0) {
                fprintf(stderr, "LZW compression failed\n");
                return 1;
            }
            break;
            
        case COMPRESS_HUFFMAN:
            result_size = compress_huffman(input_buffer, input_len, output_buffer, sizeof(output_buffer));
            if (result_size < 0) {
                fprintf(stderr, "Huffman compression failed\n");
                return 1;
            }
            break;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %d bytes\n", result_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result_size / input_len) * 100);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result_size; i++) {
        printf("%02x ", (unsigned char)output_buffer[i]);