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
    struct compression_result res = {0};
    res.original_size = len;
    res.compressed_size = len;
    res.type = COMP_NONE;
    return res;
}

static struct compression_result compress_rle(const char *input, size_t len) {
    struct compression_result res = {0};
    res.original_size = len;
    res.type = COMP_RLE;
    
    if (len == 0) {
        res.compressed_size = 0;
        return res;
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
    
    res.compressed_size = (compressed < len) ? compressed : len;
    return res;
}

static struct compression_result compress_lzw(const char *input, size_t len) {
    struct compression_result res = {0};
    res.original_size = len;
    res.type = COMP_LZW;
    
    if (len < 16) {
        res.compressed_size = len;
        return res;
    }
    
    res.compressed_size = len * 3 / 4;
    if (res.compressed_size > len) {
        res.compressed_size = len;
    }
    return res;
}

static struct compression_result compress_huffman(const char *input, size_t len) {
    struct compression_result res = {0};
    res.original_size = len;
    res.type = COMP_HUFFMAN;
    
    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)input[i]]++;
    }
    
    int unique_chars = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) unique_chars++;
    }
    
    size_t header_size = unique_chars * 5 + 32;
    size_t data_size = len * 7 / 8;
    res.compressed_size = header_size + data_size;
    
    if (res.compressed_size > len) {
        res.compressed_size = len;
    }
    return res;
}

static void print_result(const struct compression_result *res) {
    const char *type_str = "Unknown";
    switch (res->type) {
        case COMP_NONE: type_str = "None"; break;
        case COMP_RLE: type_str = "RLE"; break;
        case COMP_LZW: type_str = "LZW"; break;
        case COMP_HUFFMAN: type_str = "Huffman"; break;
    }
    
    double ratio = (res->original_size > 0) ? 
        (double)res->compressed_size / res->original_size * 100.0 : 0.0;
    
    printf("Compression: %s\n", type_str);
    printf("Original size: %zu bytes\n", res->original_size);
    printf("Compressed size: %zu bytes\n", res->compressed_size);
    printf("Compression ratio: %.1f%%\n", ratio);
    printf("\n");
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
    
    printf("Testing compression algorithms on %zu bytes:\n\n", len);
    
    for (enum compression_type type = COMP_NONE; type <= COMP_HUFFMAN; type++) {
        struct compression_result res;
        
        switch (type) {
            case COMP_NONE:
                res = compress_none(input_buffer, len);
                break;
            case COMP_RLE:
                res = compress_rle(input_buffer, len);
                break;
            case COMP_LZW:
                res = compress_lzw(input_buffer, len);
                break;
            case COMP_HUFFMAN:
                res = compress_huffman(input_buffer, len);
                break;
            default:
                continue;
        }
        
        print_result(&res);
    }
    
    return 0;
}