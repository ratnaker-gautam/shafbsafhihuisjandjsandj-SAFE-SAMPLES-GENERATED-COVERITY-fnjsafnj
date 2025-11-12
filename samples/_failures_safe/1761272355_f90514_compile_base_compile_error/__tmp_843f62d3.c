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
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < len && out_pos < out_len - 3) {
        char current = input[i];
        size_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 1) {
            output[out_pos++] = current;
            output[out_pos++] = current;
            output[out_pos++] = (char)count;
        } else {
            output[out_pos++] = current;
        }
        
        i += count;
    }
    
    if (out_pos >= out_len) return -1;
    output[out_pos] = '\0';
    return (int)out_pos;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    size_t out_pos = 0;
    char dict[256][16];
    size_t dict_size = 0;
    
    for (int i = 0; i < 256; i++) {
        dict[i][0] = (char)i;
        dict[i][1] = '\0';
    }
    dict_size = 256;
    
    size_t i = 0;
    while (i < len && out_pos < out_len - 2) {
        char current[16] = {0};
        current[0] = input[i];
        size_t current_len = 1;
        
        for (size_t j = i + 1; j < len && current_len < 15; j++) {
            current[current_len] = input[j];
            current[current_len + 1] = '\0';
            
            int found = 0;
            for (size_t k = 0; k < dict_size; k++) {
                if (strcmp(dict[k], current) == 0) {
                    found = 1;
                    break;
                }
            }
            
            if (!found) break;
            current_len++;
        }
        
        for (size_t k = 0; k < dict_size; k++) {
            if (strcmp(dict[k], current) == 0) {
                if (out_pos + 2 >= out_len) return -1;
                output[out_pos++] = (char)(k >> 8);
                output[out_pos++] = (char)(k & 0xFF);
                break;
            }
        }
        
        if (dict_size < 256 && current_len < 16) {
            strcpy(dict[dict_size], current);
            dict_size++;
        }
        
        i += current_len;
    }
    
    if (out_pos >= out_len) return -1;
    output[out_pos] = '\0';
    return (int)out_pos;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_len) {
    if (input == NULL || output == NULL || out_len == 0) return -1;
    if (len == 0) return 0;
    
    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)input[i]]++;
    }
    
    size_t out_pos = 0;
    for (int i = 0; i < 256 && out_pos < out_len - 2; i++) {
        if (freq[i] > 0) {
            output[out_pos++] = (char)i;
            output[out_pos++] = (char)(freq[i] & 0xFF);
        }
    }
    
    if (out_pos >= out_len) return -1;
    output[out_pos] = '\0';
    return (int)out_pos;
}

int main(void) {
    char input[1024];
    char output[2048];
    enum compression_type method;
    int choice;
    
    printf("Enter text to compress (max 1023 chars): ");
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
    
    printf("Choose compression method:\n");
    printf("1. None\n");
    printf("2. RLE\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }