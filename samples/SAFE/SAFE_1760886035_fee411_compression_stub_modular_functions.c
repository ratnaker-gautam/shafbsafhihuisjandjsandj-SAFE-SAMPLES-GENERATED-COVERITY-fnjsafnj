//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->size && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && 
               input->data[i + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
    return 1;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int get_user_input(Buffer* input) {
    if (input == NULL) return 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char temp[MAX_INPUT_SIZE + 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) return 0;
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(temp, len)) return 0;
    
    memcpy(input->data, temp, len);
    input->size = len;
    return 1;
}

int main(void) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("RLE Compression Demo\n");
    printf("===================\n");
    
    if (!get_user_input(&original)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", original.size);
    print_buffer_hex(&original);
    
    if (!compress_rle(&original, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    if (!decompress_rle(&compressed, &decompressed)) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("\nVerification: SUCCESS - Data matches original\n");
    } else {
        printf("\nVerification: FAILED - Data corruption detected\n");
        return 1;
    }
    
    double ratio = (double)compressed.size / original.size * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}