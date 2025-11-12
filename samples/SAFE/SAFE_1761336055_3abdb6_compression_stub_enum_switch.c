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
    if (len == 0) {
        result.compressed_size = 0;
    } else {
        result.compressed_size = len + (len / 2);
        if (result.compressed_size < len) result.compressed_size = len;
    }
    result.type = COMP_RLE;
    return result;
}

static struct compression_result compress_lzw(const char *input, size_t len) {
    struct compression_result result = {0};
    result.original_size = len;
    if (len < 10) {
        result.compressed_size = len;
    } else {
        result.compressed_size = len - (len / 4);
        if (result.compressed_size > len) result.compressed_size = len;
    }
    result.type = COMP_LZW;
    return result;
}

static struct compression_result compress_huffman(const char *input, size_t len) {
    struct compression_result result = {0};
    result.original_size = len;
    if (len < 20) {
        result.compressed_size = len;
    } else {
        result.compressed_size = len - (len / 3);
        if (result.compressed_size > len) result.compressed_size = len;
    }
    result.type = COMP_HUFFMAN;
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
    double ratio = 0.0;
    if (result->original_size > 0) {
        ratio = (double)result->compressed_size / result->original_size;
    }
    printf("Type: %s\n", type_str);
    printf("Original: %zu bytes\n", result->original_size);
    printf("Compressed: %zu bytes\n", result->compressed_size);
    printf("Ratio: %.2f\n", ratio);
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
    
    printf("Available compression methods:\n");
    printf("1. None\n");
    printf("2. RLE\n");
    printf("3. LZW\n");
    printf("4. Huffman\n");
    printf("Select method (1-4): ");
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        fprintf(stderr, "Error reading choice\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    if (choice < 1 || choice > 4) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    enum compression_type selected_type = COMP_NONE;
    switch (choice) {
        case 1: selected_type = COMP_NONE; break;
        case 2: selected_type = COMP_RLE; break;
        case 3: selected_type = COMP_LZW; break;
        case 4: selected_type = COMP_HUFFMAN; break;
    }
    
    struct compression_result result;
    switch (selected_type) {
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
    }
    
    printf("\nCompression Results:\n");
    print_result(&result);
    
    return 0;
}