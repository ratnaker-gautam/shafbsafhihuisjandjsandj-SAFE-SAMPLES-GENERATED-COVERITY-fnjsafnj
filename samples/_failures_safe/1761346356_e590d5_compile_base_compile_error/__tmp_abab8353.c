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

int compress_rle(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        if (out_idx + 2 >= out_len) return -1;
        
        char current = input[i];
        uint8_t count = 1;
        
        while (i + count < len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = count;
        i += count;
    }
    
    return out_idx;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    size_t dict_size = 256;
    
    for (size_t i = 0; i < len; i++) {
        if (out_idx >= out_len) return -1;
        output[out_idx++] = input[i];
        
        if (i < len - 1 && dict_size < 65536) {
            dict_size++;
        }
    }
    
    return out_idx;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_idx = 0;
    uint8_t freq[256] = {0};
    
    for (size_t i = 0; i < len; i++) {
        freq[(uint8_t)input[i]]++;
    }
    
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            if (out_idx + 2 >= out_len) return -1;
            output[out_idx++] = (char)i;
            output[out_idx++] = (char)freq[i];
        }
    }
    
    if (out_idx >= out_len) return -1;
    output[out_idx++] = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (out_idx >= out_len) return -1;
        output[out_idx++] = input[i];
    }
    
    return out_idx;
}

int main(void) {
    char input[1024];
    char output[2048];
    
    printf("Enter text to compress (max 1023 chars): ");
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
        printf("Empty input\n");
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
    
    int result;
    switch (method) {
        case COMPRESS_NONE:
            if (len >= sizeof(output)) {
                fprintf(stderr, "Output buffer too small\n");
                return 1;
            }
            memcpy(output, input, len);
            result = len;
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
            fprintf(stderr, "Unknown compression method\n");
            return 1;
    }
    
    if (result < 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", len);
    printf("Compressed size: %d bytes\n",