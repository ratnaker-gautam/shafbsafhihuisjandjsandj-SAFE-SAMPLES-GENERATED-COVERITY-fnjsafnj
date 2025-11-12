//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
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
    
    if (len == 0) {
        result.compressed_size = 0;
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
    
    if (len < 10) {
        result.compressed_size = len;
        return result;
    }
    
    result.compressed_size = len * 3 / 4;
    return result;
}

static struct compression_result compress_huffman(const char *input, size_t len) {
    struct compression_result result = {0};
    result.original_size = len;
    result.type = COMP_HUFFMAN;
    
    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[(unsigned char)input[i]]++;
    }
    
    size_t compressed = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            compressed += 10;
        }
    }
    
    compressed += len / 2;
    result.compressed_size = (compressed < len) ? compressed : len;
    return result;
}

static void print_result(const struct compression_result *result) {
    const char *type_str = "Unknown";
    switch (result->type) {
        case COMP_NONE: type_str = "None"; break;
        case COMP_RLE: type_str = "RLE"; break;
        case COMP_LZW: type_str = "LZW"; break;
        case COMP_HUFFMAN: type_str = "Huffman"; break;
    }
    
    double ratio = (result->original_size > 0) ? 
        (double)result->compressed_size / result->original_size : 0.0;
    
    printf("Compression: %s\n", type_str);
    printf("Original size: %zu bytes\n", result->original_size);
    printf("Compressed size: %zu bytes\n", result->compressed_size);
    printf("Compression ratio: %.2f\n", ratio);
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
    
    printf("Testing compression algorithms on input:\n");
    printf("Text: %s\n", input_buffer);
    printf("Length: %zu bytes\n\n", len);
    
    enum compression_type algorithms[] = {
        COMP_NONE,
        COMP_RLE,
        COMP_LZW,
        COMP_HUFFMAN
    };
    
    for (size_t i = 0; i < sizeof(algorithms)/sizeof(algorithms[0]); i++) {
        struct compression_result result;
        
        switch (algorithms[i]) {
            case COMP_NONE:
                result = compress_none(input_buffer, len);
                break;
            case COMP_RLE:
                result = compress_rle(input_buffer, len);
                break;
            case COMP_LZW:
                result = compress_lzw(input_buffer, len);
                break;
            case COMP_HUFFMAN:
                result = compress_huffman(input_buffer, len);
                break;
            default:
                continue;
        }
        
        print_result(&result);
    }
    
    return 0;
}