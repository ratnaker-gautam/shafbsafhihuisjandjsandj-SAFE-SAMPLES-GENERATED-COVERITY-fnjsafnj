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
    return 1;
}

size_t recursive_compress(const uint8_t* input, size_t pos, size_t len, uint8_t* output, size_t out_pos) {
    if (pos >= len) {
        return out_pos;
    }
    
    size_t count = 1;
    while (pos + count < len && input[pos] == input[pos + count] && count < 255) {
        count++;
    }
    
    if (count > 3 || input[pos] == 0xFF) {
        if (out_pos + 2 >= MAX_INPUT_SIZE) {
            return out_pos;
        }
        output[out_pos++] = 0xFF;
        output[out_pos++] = (uint8_t)count;
        output[out_pos++] = input[pos];
        return recursive_compress(input, pos + count, len, output, out_pos);
    } else {
        if (out_pos + 1 >= MAX_INPUT_SIZE) {
            return out_pos;
        }
        output[out_pos++] = input[pos];
        return recursive_compress(input, pos + 1, len, output, out_pos);
    }
}

size_t recursive_decompress(const uint8_t* input, size_t pos, size_t len, uint8_t* output, size_t out_pos) {
    if (pos >= len) {
        return out_pos;
    }
    
    if (input[pos] == 0xFF && pos + 2 < len) {
        uint8_t count = input[pos + 1];
        uint8_t value = input[pos + 2];
        
        if (out_pos + count >= MAX_INPUT_SIZE) {
            return out_pos;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_pos++] = value;
        }
        return recursive_decompress(input, pos + 3, len, output, out_pos);
    } else {
        if (out_pos + 1 >= MAX_INPUT_SIZE) {
            return out_pos;
        }
        output[out_pos++] = input[pos];
        return recursive_decompress(input, pos + 1, len, output, out_pos);
    }
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    temp[strcspn(temp, "\n")] = 0;
    
    size_t input_len = strlen(temp);
    if (!validate_input(temp, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(input.data, temp, input_len);
    input.length = input_len;
    
    compressed.length = recursive_compress(input.data, 0, input.length, compressed.data, 0);
    
    if (compressed.length >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Compression buffer overflow\n");
        return 1;
    }
    
    decompressed.length = recursive_decompress(compressed.data, 0, compressed.length, decompressed.data, 0);
    
    if (decompressed.length >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Decompression buffer overflow\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           (input.length > 0) ? (100.0 * (input.length - compressed.length) / input.length) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.length; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    if (input.length != decompressed.length || memcmp(input.data, decompressed.data, input.length) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    
    return 0;
}