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
    size_t out_pos = 0;
    for (size_t i = 0; i < len && out_pos < out_size - 2; ) {
        char current = input[i];
        uint8_t count = 1;
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        if (out_pos + 2 >= out_size) return -1;
        output[out_pos++] = (char)count;
        output[out_pos++] = current;
        i += count;
    }
    return (int)out_pos;
}

int compress_lzw(const char *input, size_t len, char *output, size_t out_size) {
    if (input == NULL || output == NULL || out_size == 0) return -1;
    if (len == 0) return 0;
    size_t out_pos = 0;
    char dict[256][16];
    size_t dict_size = 256;
    for (int i = 0; i < 256; i++) {
        dict[i][0] = (char)i;
        dict[i][1] = '\0';
    }
    char current[16] = {0};
    current[0] = input[0];
    for (size_t i = 1; i < len && out_pos < out_size - 1; i++) {
        char next[16];
        strncpy(next, current, 15);
        size_t cur_len = strlen(next);
        if (cur_len < 15) next[cur_len] = input[i];
        next[cur_len + 1] = '\0';
        int found = 0;
        for (size_t j = 0; j < dict_size; j++) {
            if (strcmp(dict[j], next) == 0) {
                strncpy(current, next, 15);
                found = 1;
                break;
            }
        }
        if (!found) {
            for (size_t j = 0; j < dict_size; j++) {
                if (strcmp(dict[j], current) == 0) {
                    if (out_pos >= out_size) return -1;
                    output[out_pos++] = (char)(j & 0xFF);
                    break;
                }
            }
            if (dict_size < 256) {
                strncpy(dict[dict_size], next, 15);
                dict_size++;
            }
            current[0] = input[i];
            current[1] = '\0';
        }
    }
    for (size_t j = 0; j < dict_size; j++) {
        if (strcmp(dict[j], current) == 0) {
            if (out_pos >= out_size) return -1;
            output[out_pos++] = (char)(j & 0xFF);
            break;
        }
    }
    return (int)out_pos;
}

int compress_huffman(const char *input, size_t len, char *output, size_t out_size) {
    if (input == NULL || output == NULL || out_size < 2) return -1;
    if (len == 0) {
        output[0] = 0;
        return 1;
    }
    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)input[i]]++;
    }
    size_t out_pos = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            if (out_pos + 2 >= out_size) return -1;
            output[out_pos++] = (char)i;
            output[out_pos++] = (char)(freq[i] > 255 ? 255 : freq[i]);
        }
    }
    if (out_pos >= out_size) return -1;
    output[out_pos++] = 0;
    for (size_t i = 0; i < len && out_pos < out_size; i++) {
        output[out_pos++] = input[i];
    }
    return (int)out_pos;
}

int main(void) {
    char input[256];
    char output[512];
    printf("Enter text to compress (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
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
    printf("Select compression type:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: LZW\n");
    printf("4: Huffman\n");
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr