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

static int validate_input(const char *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > 1024 * 1024) return 0;
    return 1;
}

static struct compression_result compress_none(const char *input, size_t size) {
    struct compression_result result = {0};
    result.original_size = size;
    result.compressed_size = size;
    result.type = COMP_NONE;
    return result;
}

static struct compression_result compress_rle(const char *input, size_t size) {
    struct compression_result result = {0};
    result.original_size = size;
    result.type = COMP_RLE;
    
    if (size < 2) {
        result.compressed_size = size;
        return result;
    }
    
    size_t compressed = 0;
    for (size_t i = 0; i < size; ) {
        char current = input[i];
        size_t count = 1;
        
        while (i + count < size && input[i + count] == current && count < 255) {
            count++;
        }
        
        compressed += 2;
        i += count;
    }
    
    result.compressed_size = (compressed < size) ? compressed : size;
    return result;
}

static struct compression_result compress_lzw(const char *input, size_t size) {
    struct compression_result result = {0};
    result.original_size = size;
    result.type = COMP_LZW;
    
    if (size < 4) {
        result.compressed_size = size;
        return result;
    }
    
    size_t compressed = size / 2 + 8;
    result.compressed_size = (compressed < size) ? compressed : size;
    return result;
}

static struct compression_result compress_huffman(const char *input, size_t size) {
    struct compression_result result = {0};
    result.original_size = size;
    result.type = COMP_HUFFMAN;
    
    if (size < 8) {
        result.compressed_size = size;
        return result;
    }
    
    int freq[256] = {0};
    for (size_t i = 0; i < size; i++) {
        freq[(unsigned char)input[i]]++;
    }
    
    size_t compressed = 256;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) compressed += 5;
    }
    compressed += size * 3 / 4;
    
    result.compressed_size = (compressed < size) ? compressed : size;
    return result;
}

static struct compression_result compress_data(const char *input, size_t size, enum compression_type method) {
    struct compression_result result = {0};
    
    switch (method) {
        case COMP_NONE:
            result = compress_none(input, size);
            break;
        case COMP_RLE:
            result = compress_rle(input, size);
            break;
        case COMP_LZW:
            result = compress_lzw(input, size);
            break;
        case COMP_HUFFMAN:
            result = compress_huffman(input, size);
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
    const char *type_names[] = {"None", "RLE", "LZW", "Huffman"};
    
    printf("Compression type: %s\n", type_names[result->type]);
    printf("Original size: %zu bytes\n", result->original_size);
    printf("Compressed size: %zu bytes\n", result->compressed_size);
    
    if (result->original_size > 0) {
        double ratio = (double)result->compressed_size / result->original_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
}

int main(void) {
    char input_buffer[1024];
    size_t input_size;
    
    printf("Enter data to compress (max 1023 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_size = strlen(input_buffer);
    if (input_size > 0 && input_buffer[input_size - 1] == '\n') {
        input_buffer[--input_size] = '\0';
    }
    
    if (!validate_input(input_buffer, input_size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Available compression methods:\n");
    printf("1. None\n");
    printf("2. RLE\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Select method (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    while (getchar() != '\n');