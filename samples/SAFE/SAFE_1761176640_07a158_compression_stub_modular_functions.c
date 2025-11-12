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
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < MAX_OUTPUT_SIZE - 2; i++) {
        uint8_t current = (uint8_t)input[i];
        size_t count = 1;
        
        while (i + 1 < len && input[i] == input[i + 1] && count < UINT8_MAX) {
            count++;
            i++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) break;
        
        result.data[out_idx++] = current;
        result.data[out_idx++] = (uint8_t)count;
    }
    
    result.size = out_idx;
    return result;
}

Buffer decompress_data(const uint8_t* input, size_t len) {
    Buffer result = {0};
    if (input == NULL || len == 0 || len % 2 != 0) return result;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < MAX_OUTPUT_SIZE; i += 2) {
        uint8_t character = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
            result.data[out_idx++] = character;
        }
    }
    
    result.size = out_idx;
    return result;
}

int compare_buffers(const Buffer* a, const Buffer* b) {
    if (a->size != b->size) return 0;
    return memcmp(a->data, b->data, a->size) == 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < buf->size) printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
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
    
    Buffer compressed = compress_data(input, len);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / len * 100.0f);
    
    printf("Compressed data (hex):\n");
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(decompressed.data[i]);
    }
    printf("\n");
    
    if (compare_buffers(&(Buffer){(uint8_t*)input, len}, &decompressed)) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}