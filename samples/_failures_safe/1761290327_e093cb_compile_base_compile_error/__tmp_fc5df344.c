//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

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

static int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL && size > 0) return 0;
    if (size > SIZE_MAX / 2) return 0;
    return 1;
}

static struct compression_result compress_none(const uint8_t *input, size_t input_size) {
    struct compression_result result = {0};
    result.original_size = input_size;
    result.compressed_size = input_size;
    result.type = COMP_NONE;
    return result;
}

static struct compression_result compress_rle(const uint8_t *input, size_t input_size) {
    struct compression_result result = {0};
    result.original_size = input_size;
    result.type = COMP_RLE;
    
    if (input_size == 0) {
        result.compressed_size = 0;
        return result;
    }
    
    size_t compressed = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        compressed += 2;
        i += count;
        
        if (compressed > SIZE_MAX - 2) {
            result.compressed_size = input_size;
            return result;
        }
    }
    
    result.compressed_size = (compressed < input_size) ? compressed : input_size;
    return result;
}

static struct compression_result compress_lzw(const uint8_t *input, size_t input_size) {
    struct compression_result result = {0};
    result.original_size = input_size;
    result.type = COMP_LZW;
    
    if (input_size < 10) {
        result.compressed_size = input_size;
        return result;
    }
    
    size_t estimated = input_size / 2 + 10;
    result.compressed_size = (estimated < input_size) ? estimated : input_size;
    return result;
}

static struct compression_result compress_huffman(const uint8_t *input, size_t input_size) {
    struct compression_result result = {0};
    result.original_size = input_size;
    result.type = COMP_HUFFMAN;
    
    if (input_size == 0) {
        result.compressed_size = 0;
        return result;
    }
    
    int freq[256] = {0};
    for (size_t i = 0; i < input_size; i++) {
        freq[input[i]]++;
    }
    
    size_t unique = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) unique++;
    }
    
    size_t header_size = unique * 5 + 10;
    size_t data_estimate = input_size * 3 / 4;
    size_t total = header_size + data_estimate;
    
    result.compressed_size = (total < input_size) ? total : input_size;
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
}

int main(void) {
    uint8_t test_data[256];
    size_t data_size = 0;
    
    printf("Enter data size (1-256): ");
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (data_size < 1 || data_size > 256) {
        printf("Size must be between 1 and 256\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid byte value\n");
            return 1;
        }
        if (value < 0 || value > 255) {
            printf("Byte value must be between 0 and 255\n");
            return 1;
        }
        test_data[i] = (uint8_t)value;
    }
    
    if (!validate_input(test_data, data_size)) {
        printf("Input validation failed\n");
        return 1;
    }
    
    printf("\nCompression Results:\n");
    printf("===================\n");
    
    struct compression