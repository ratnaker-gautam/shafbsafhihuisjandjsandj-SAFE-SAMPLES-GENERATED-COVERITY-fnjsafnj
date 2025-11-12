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
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

Buffer compress_rle(const char* input, size_t len) {
    Buffer result = {0};
    if (!validate_input(input, len)) return result;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < len && out_idx < MAX_OUTPUT_SIZE - 2) {
        char current = input[in_idx];
        uint8_t count = 1;
        
        while (in_idx + count < len && count < UINT8_MAX && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
        
        result.data[out_idx++] = count;
        result.data[out_idx++] = (uint8_t)current;
        in_idx += count;
    }
    
    result.size = out_idx;
    return result;
}

Buffer decompress_rle(const char* input, size_t len) {
    Buffer result = {0};
    if (!validate_input(input, len)) return result;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx + 1 < len && out_idx < MAX_OUTPUT_SIZE) {
        uint8_t count = (uint8_t)input[in_idx++];
        char value = (char)input[in_idx++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t i = 0; i < count && out_idx < MAX_OUTPUT_SIZE; i++) {
            result.data[out_idx++] = (uint8_t)value;
        }
    }
    
    result.size = out_idx;
    return result;
}

void print_buffer(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        if (i > 0) printf(" ");
        printf("%02X", buf->data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
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
    
    printf("Compressed (%zu bytes): ", compressed.size);
    print_buffer(&compressed);
    
    Buffer decompressed = decompress_rle((char*)compressed.data, compressed.size);
    if (decompressed.size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", (char)decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == len && 
        memcmp(input, decompressed.data, len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}