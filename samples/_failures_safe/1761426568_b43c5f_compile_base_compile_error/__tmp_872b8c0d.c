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

static int validate_input(const char *data, size_t len) {
    if (data == NULL) return 0;
    if (len == 0) return 0;
    if (len > 1024 * 1024) return 0;
    return 1;
}

static struct compression_result compress_none(const char *input, size_t len) {
    struct compression_result result = {0};
    result.original_size = len;
    result.compressed_size = len;
    result.type = COMP_NONE;
    return result;
}

static struct compression_result compress_rle(const char *input, size_t len) {
    struct compression_result result = {0};
    result.original_size = len;
    result.type = COMP_RLE;
    
    if (len < 2) {
        result.compressed_size = len;
        return result;
    }
    
    size_t compressed = 0;
    for (size_t i = 0; i < len; ) {
        char current = input[i];
        size_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        compressed += 2;
        i += count;
    }
    
    result.compressed_size = (compressed < len) ? compressed : len;
    return result;
}

static struct compression_result compress_lzw(const char *input, size_t len) {
    struct compression_result result = {0};
    result.original_size = len;
    result.type = COMP_LZW;
    
    if (len < 3) {
        result.compressed_size = len;
        return result;
    }
    
    size_t unique_chars = 0;
    int seen[256] = {0};
    for (size_t i = 0; i < len; i++) {
        unsigned char c = input[i];
        if (!seen[c]) {
            seen[c] = 1;
            unique_chars++;
        }
    }
    
    size_t dict_size = unique_chars;
    size_t compressed = dict_size * 2;
    
    for (size_t i = 0; i < len - 1; ) {
        size_t pattern_len = 2;
        while (i + pattern_len <= len && pattern_len < 16) {
            pattern_len++;
        }
        compressed += 2;
        i += pattern_len - 1;
    }
    
    result.compressed_size = (compressed < len) ? compressed : len;
    return result;
}

static struct compression_result compress_huffman(const char *input, size_t len) {
    struct compression_result result = {0};
    result.original_size = len;
    result.type = COMP_HUFFMAN;
    
    if (len < 4) {
        result.compressed_size = len;
        return result;
    }
    
    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        unsigned char c = input[i];
        freq[c]++;
    }
    
    size_t unique_symbols = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) unique_symbols++;
    }
    
    size_t header_size = unique_symbols * 3 + 10;
    size_t compressed_data = (len * 3) / 4;
    result.compressed_size = header_size + compressed_data;
    
    if (result.compressed_size >= len) {
        result.compressed_size = len;
    }
    
    return result;
}

static void print_result(const struct compression_result *result, const char *label) {
    if (result == NULL) return;
    
    double ratio = 0.0;
    if (result->original_size > 0) {
        ratio = (double)result->compressed_size / result->original_size * 100.0;
    }
    
    const char *type_str = "Unknown";
    switch (result->type) {
        case COMP_NONE: type_str = "None"; break;
        case COMP_RLE: type_str = "RLE"; break;
        case COMP_LZW: type_str = "LZW"; break;
        case COMP_HUFFMAN: type_str = "Huffman"; break;
    }
    
    printf("%s: %s compression - Original: %zu, Compressed: %zu, Ratio: %.1f%%\n",
           label, type_str, result->original_size, result->compressed_size, ratio);
}

int main(void) {
    char input_buffer[1024];
    printf("Enter text to compress (max 1023 chars): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input_buffer, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Input length: %zu characters\n\n", len