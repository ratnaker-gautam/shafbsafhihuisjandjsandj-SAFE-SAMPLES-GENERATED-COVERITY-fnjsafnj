//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_HUFFMAN,
    COMPRESS_LZW
};

int compress_rle(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < out_len - 2) {
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (char)count;
        i += count;
    }
    
    if (out_idx >= out_len) return -1;
    output[out_idx] = '\0';
    return (int)out_idx;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < out_len - 1; i++) {
        char c = input[i];
        if (isprint(c)) {
            output[out_idx++] = c;
        } else {
            output[out_idx++] = '?';
        }
    }
    
    if (out_idx >= out_len) return -1;
    output[out_idx] = '\0';
    return (int)out_idx;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < out_len - 1; i++) {
        output[out_idx++] = input[i];
    }
    
    if (out_idx >= out_len) return -1;
    output[out_idx] = '\0';
    return (int)out_idx;
}

int main(void) {
    char input[256];
    char output[512];
    int choice;
    
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    printf("Choose compression method:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: Huffman\n");
    printf("4: LZW\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int result;
    enum compression_type comp_type;
    
    switch (choice) {
        case 1:
            comp_type = COMPRESS_NONE;
            break;
        case 2:
            comp_type = COMPRESS_RLE;
            break;
        case 3:
            comp_type = COMPRESS_HUFFMAN;
            break;
        case 4:
            comp_type = COMPRESS_LZW;
            break;
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    switch (comp_type) {
        case COMPRESS_NONE:
            if (len >= sizeof(output)) {
                fprintf(stderr, "Output buffer too small\n");
                return 1;
            }
            memcpy(output, input, len);
            output[len] = '\0';
            result = (int)len;
            break;
            
        case COMPRESS_RLE:
            result = compress_rle(input, len, output, sizeof(output));
            break;
            
        case COMPRESS_HUFFMAN:
            result = compress_huffman(input, len, output, sizeof(output));
            break;
            
        case COMPRESS_LZW:
            result = compress_lzw(input, len, output, sizeof(output));
            break;
    }
    
    if (result < 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original length: %zu\n", len);
    printf("Compressed length: %d\n", result);
    printf("Compressed data: %s\n", output);
    
    return 0;
}