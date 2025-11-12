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
    size_t i = 0;
    
    while (i < len && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && count < UINT8_MAX && 
               input[i + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
        
        result.data[out_idx++] = (uint8_t)count;
        result.data[out_idx++] = current;
        result.size = out_idx;
        
        i += count;
    }
    
    return result;
}

Buffer decompress_rle(const uint8_t* input, size_t len) {
    Buffer result = {0};
    if (input == NULL || len == 0 || len % 2 != 0) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < MAX_OUTPUT_SIZE) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
            result.data[out_idx++] = value;
        }
        result.size = out_idx;
    }
    
    return result;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < buf->size) printf(" ");
    }
    if (buf->size % 16 != 0) printf("\n");
}

void print_buffer_text(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        if (buf->data[i] >= 32 && buf->data[i] <= 126) {
            printf("%c", buf->data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
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
    
    printf("Original text: %s\n", input);
    printf("Original length: %zu bytes\n", len);
    
    Buffer compressed = compress_rle(input, len);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed text: ");
    print_buffer_text(&decompressed);
    printf("Decompressed length: %zu bytes\n", decompressed.size);
    
    if (len == decompressed.size && memcmp(input, decompressed.data, len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    double ratio = (double)compressed.size / len * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}