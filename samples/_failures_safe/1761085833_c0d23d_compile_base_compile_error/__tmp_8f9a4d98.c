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

static int validate_input(const char *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > 1024 * 1024) return 0;
    return 1;
}

static struct compression_result compress_none(const char *input, size_t size) {
    struct compression_result result;
    result.original_size = size;
    result.compressed_size = size;
    result.type = COMP_NONE;
    return result;
}

static struct compression_result compress_rle(const char *input, size_t size) {
    struct compression_result result;
    result.original_size = size;
    result.type = COMP_RLE;
    
    if (size == 0) {
        result.compressed_size = 0;
        return result;
    }
    
    size_t compressed = 0;
    size_t i = 0;
    
    while (i < size) {
        char current = input[i];
        size_t count = 1;
        
        while (i + count < size && input[i + count] == current && count < 255) {
            count++;
        }
        
        compressed += 2;
        i += count;
    }
    
    result.compressed_size = compressed;
    return result;
}

static struct compression_result compress_lzw(const char *input, size_t size) {
    struct compression_result result;
    result.original_size = size;
    result.type = COMP_LZW;
    
    if (size < 10) {
        result.compressed_size = size + 4;
    } else {
        result.compressed_size = size * 3 / 4;
    }
    
    if (result.compressed_size < 8) {
        result.compressed_size = 8;
    }
    
    return result;
}

static struct compression_result compress_huffman(const char *input, size_t size) {
    struct compression_result result;
    result.original_size = size;
    result.type = COMP_HUFFMAN;
    
    int freq[256] = {0};
    for (size_t i = 0; i < size; i++) {
        freq[(unsigned char)input[i]]++;
    }
    
    size_t compressed = 0;
    int unique_chars = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            unique_chars++;
        }
    }
    
    compressed = unique_chars * 3 + size * 2 / 3;
    if (compressed < size) {
        result.compressed_size = compressed;
    } else {
        result.compressed_size = size;
    }
    
    return result;
}

static struct compression_result perform_compression(const char *data, size_t size, enum compression_type type) {
    struct compression_result result;
    
    switch (type) {
        case COMP_NONE:
            result = compress_none(data, size);
            break;
        case COMP_RLE:
            result = compress_rle(data, size);
            break;
        case COMP_LZW:
            result = compress_lzw(data, size);
            break;
        case COMP_HUFFMAN:
            result = compress_huffman(data, size);
            break;
        default:
            result.original_size = size;
            result.compressed_size = size;
            result.type = COMP_NONE;
            break;
    }
    
    return result;
}

static void print_result(const struct compression_result *result) {
    const char *type_names[] = {
        "None",
        "RLE",
        "LZW",
        "Huffman"
    };
    
    double ratio = 0.0;
    if (result->original_size > 0) {
        ratio = (double)result->compressed_size / result->original_size * 100.0;
    }
    
    printf("Compression: %s\n", type_names[result->type]);
    printf("Original size: %zu bytes\n", result->original_size);
    printf("Compressed size: %zu bytes\n", result->compressed_size);
    printf("Compression ratio: %.1f%%\n", ratio);
}

int main(void) {
    char input_buffer[1024];
    printf("Enter text to compress (max 1023 chars): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("\nAvailable compression methods:\n");
    printf("1. None\n");
    printf("2. Run-Length Encoding (RLE)\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Select method (1-4): ");
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL