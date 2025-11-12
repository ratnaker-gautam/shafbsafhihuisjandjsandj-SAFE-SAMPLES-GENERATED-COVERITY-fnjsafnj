//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0) return 0;
    if (len > MAX_INPUT_SIZE) return 0;
    return 1;
}

Buffer compress_rle(const char* input, size_t len) {
    Buffer result = {0};
    if (!validate_input(input, len)) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && count < UINT8_MAX && 
               input[i + count] == current && 
               out_idx < MAX_OUTPUT_SIZE - 2) {
            count++;
        }
        
        if (count >= 3) {
            result.data[out_idx++] = 0xFF;
            result.data[out_idx++] = current;
            result.data[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                result.data[out_idx++] = input[i + j];
            }
            i += count;
        }
    }
    
    result.size = out_idx;
    return result;
}

Buffer decompress_rle(const uint8_t* input, size_t len) {
    Buffer result = {0};
    if (input == NULL || len == 0 || len > MAX_OUTPUT_SIZE) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < MAX_OUTPUT_SIZE) {
        if (i + 2 < len && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count > MAX_OUTPUT_SIZE) break;
            
            for (uint8_t j = 0; j < count; j++) {
                result.data[out_idx++] = value;
            }
            i += 3;
        } else {
            result.data[out_idx++] = input[i++];
        }
    }
    
    result.size = out_idx;
    return result;
}

void print_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < len) printf(" ");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", len);
    
    Buffer compressed = compress_rle(input, len);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data (hex):\n");
    print_hex(compressed.data, compressed.size);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    decompressed.data[decompressed.size] = '\0';
    printf("Decompressed: %s\n", decompressed.data);
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (len == decompressed.size && memcmp(input, decompressed.data, len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}