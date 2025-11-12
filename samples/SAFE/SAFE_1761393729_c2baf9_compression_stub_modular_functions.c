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

Buffer compress_data(const char* input, size_t len) {
    Buffer result = {0};
    if (!validate_input(input, len)) return result;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < len && out_pos < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = (uint8_t)input[in_pos];
        size_t count = 1;
        
        while (in_pos + count < len && count < 255 && 
               input[in_pos + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_pos + 2 >= MAX_OUTPUT_SIZE) break;
            result.data[out_pos++] = 0xFF;
            result.data[out_pos++] = (uint8_t)count;
            result.data[out_pos++] = current;
        } else {
            for (size_t i = 0; i < count && out_pos < MAX_OUTPUT_SIZE; i++) {
                result.data[out_pos++] = current;
            }
        }
        
        in_pos += count;
    }
    
    result.size = out_pos;
    return result;
}

Buffer decompress_data(const uint8_t* input, size_t len) {
    Buffer result = {0};
    if (input == NULL || len == 0) return result;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < len && out_pos < MAX_OUTPUT_SIZE) {
        if (input[in_pos] == 0xFF && in_pos + 2 < len) {
            uint8_t count = input[in_pos + 1];
            uint8_t value = input[in_pos + 2];
            
            if (out_pos + count > MAX_OUTPUT_SIZE) break;
            
            for (uint8_t i = 0; i < count; i++) {
                result.data[out_pos++] = value;
            }
            
            in_pos += 3;
        } else {
            result.data[out_pos++] = input[in_pos++];
        }
    }
    
    result.size = out_pos;
    return result;
}

int compare_buffers(const uint8_t* a, const uint8_t* b, size_t len) {
    if (a == NULL || b == NULL) return 0;
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
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
    
    printf("Original size: %zu bytes\n", len);
    
    Buffer compressed = compress_data(input, len);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / len * 100.0f);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (compare_buffers((uint8_t*)input, decompressed.data, len)) {
        printf("Decompression verified: OK\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    printf("First 50 bytes of compressed data (hex): ");
    for (size_t i = 0; i < 50 && i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}