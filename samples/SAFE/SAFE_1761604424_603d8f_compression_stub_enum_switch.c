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

int rle_compress(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || len == 0 || out_len < 2) return -1;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < out_len - 2; ) {
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 >= out_len) return -1;
        output[out_idx++] = current;
        output[out_idx++] = (char)count;
        i += count;
    }
    
    if (out_idx < out_len) {
        output[out_idx] = '\0';
    }
    return (int)out_idx;
}

int lzw_compress(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || len == 0 || out_len < len + 1) return -1;
    
    for (size_t i = 0; i < len && i < out_len - 1; i++) {
        output[i] = input[i] + 1;
    }
    
    if (len < out_len) {
        output[len] = '\0';
    }
    return (int)len;
}

int huffman_compress(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || len == 0 || out_len < len + 1) return -1;
    
    for (size_t i = 0; i < len && i < out_len - 1; i++) {
        output[i] = input[i] ^ 0x55;
    }
    
    if (len < out_len) {
        output[len] = '\0';
    }
    return (int)len;
}

int compress_data(enum compression_type type, const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || len == 0 || out_len == 0) return -1;
    
    switch (type) {
        case COMP_NONE:
            if (out_len < len + 1) return -1;
            memcpy(output, input, len);
            output[len] = '\0';
            return (int)len;
            
        case COMP_RLE:
            return rle_compress(input, len, output, out_len);
            
        case COMP_LZW:
            return lzw_compress(input, len, output, out_len);
            
        case COMP_HUFFMAN:
            return huffman_compress(input, len, output, out_len);
            
        default:
            return -1;
    }
}

int main(void) {
    char input[256];
    char output[512];
    
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    printf("Select compression method:\n");
    printf("0: None\n");
    printf("1: RLE\n");
    printf("2: LZW\n");
    printf("3: Huffman\n");
    printf("Choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    enum compression_type comp_type;
    switch (choice) {
        case 0: comp_type = COMP_NONE; break;
        case 1: comp_type = COMP_RLE; break;
        case 2: comp_type = COMP_LZW; break;
        case 3: comp_type = COMP_HUFFMAN; break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    int result = compress_data(comp_type, input, len, output, sizeof(output));
    if (result < 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %d bytes\n", result);
    printf("Compression ratio: %.2f%%\n", (float)result / len * 100.0f);
    
    printf("Compressed data (hex): ");
    for (int i = 0; i < result && i < 50; i++) {
        printf("%02x ", (unsigned char)output[i]);
    }
    if (result > 50) printf("...");
    printf("\n");
    
    return 0;
}