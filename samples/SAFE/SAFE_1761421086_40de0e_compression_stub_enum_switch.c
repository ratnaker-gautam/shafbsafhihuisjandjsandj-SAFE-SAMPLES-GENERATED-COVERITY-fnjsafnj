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

int compress_rle(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < out_len - 2) {
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    if (i < len) return -1;
    return out_idx;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < out_len - 1) {
        output[out_idx++] = input[i++];
    }
    
    if (i < len) return -1;
    return out_idx;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < out_len - 1) {
        output[out_idx++] = input[i++] ^ 0x55;
    }
    
    if (i < len) return -1;
    return out_idx;
}

int main(void) {
    char input[256];
    char output[512];
    
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
        printf("No input provided\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: LZW\n");
    printf("4: Huffman\n");
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
    
    int result;
    switch (method) {
        case COMPRESS_NONE:
            if (len < sizeof(output)) {
                memcpy(output, input, len);
                result = len;
            } else {
                result = -1;
            }
            break;
            
        case COMPRESS_RLE:
            result = compress_rle(input, len, output, sizeof(output));
            break;
            
        case COMPRESS_LZW:
            result = compress_lzw(input, len, output, sizeof(output));
            break;
            
        case COMPRESS_HUFFMAN:
            result = compress_huffman(input, len, output, sizeof(output));
            break;
            
        default:
            result = -1;
            break;
    }
    
    if (result < 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %d bytes\n", result);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)result / len) * 100);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result && i < 32; i++) {
        printf("%02x ", (unsigned char)output[i]);
    }
    printf("\n");
    
    return 0;
}