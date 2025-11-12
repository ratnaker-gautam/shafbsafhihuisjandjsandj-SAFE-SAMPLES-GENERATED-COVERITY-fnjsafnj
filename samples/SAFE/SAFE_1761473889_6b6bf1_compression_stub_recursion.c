//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
    }
    return 1;
}

size_t recursive_compress(const uint8_t* input, size_t pos, size_t len, uint8_t* output, size_t out_pos) {
    if (pos >= len) {
        return out_pos;
    }
    
    uint8_t current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (count > 3 || current == '\\' || current == '|') {
        output[out_pos] = '\\';
        output[out_pos + 1] = (uint8_t)count;
        output[out_pos + 2] = current;
        return recursive_compress(input, pos + count, len, output, out_pos + 3);
    } else {
        for (size_t i = 0; i < count; i++) {
            output[out_pos + i] = current;
        }
        return recursive_compress(input, pos + count, len, output, out_pos + count);
    }
}

size_t recursive_decompress(const uint8_t* input, size_t pos, size_t len, uint8_t* output, size_t out_pos) {
    if (pos >= len) {
        return out_pos;
    }
    
    if (input[pos] == '\\' && pos + 2 < len) {
        uint8_t count = input[pos + 1];
        uint8_t value = input[pos + 2];
        
        if (out_pos + count > MAX_INPUT_SIZE) {
            return 0;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_pos + i] = value;
        }
        return recursive_decompress(input, pos + 3, len, output, out_pos + count);
    } else {
        output[out_pos] = input[pos];
        return recursive_decompress(input, pos + 1, len, output, out_pos + 1);
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (!validate_input(input_str, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(original.data, input_str, input_len);
    original.length = input_len;
    
    compressed.length = recursive_compress(original.data, 0, original.length, compressed.data, 0);
    if (compressed.length == 0 || compressed.length > MAX_INPUT_SIZE) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    decompressed.length = recursive_decompress(compressed.data, 0, compressed.length, decompressed.data, 0);
    if (decompressed.length == 0 || decompressed.length > MAX_INPUT_SIZE) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed.length != original.length || memcmp(original.data, decompressed.data, original.length) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", original.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.length / original.length) * 100);
    printf("Verification: PASSED\n");
    
    return 0;
}