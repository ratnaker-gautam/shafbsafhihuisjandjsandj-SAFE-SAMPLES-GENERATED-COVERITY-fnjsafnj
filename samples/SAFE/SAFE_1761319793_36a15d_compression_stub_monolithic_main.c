//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 4096
#define MAX_OUTPUT_SIZE 8192

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            return 0;
        }
        
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
    return 1;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    
    if (input->size % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            return 0;
        }
        
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
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (buf->size > 0) {
        printf("\n");
    }
}

int main() {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %zu bytes): ", MAX_INPUT_SIZE);
    
    int c;
    size_t pos = 0;
    
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE) {
        if (isprint(c) || c == ' ' || c == '\t') {
            input.data[pos++] = (uint8_t)c;
        }
    }
    
    if (pos == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    input.size = pos;
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    for (size_t i = 0; i < input.size; i++) {
        putchar(input.data[i]);
    }
    printf("\n\n");
    
    printf("Original data (hex):\n");
    print_buffer_hex(&input);
    printf("\n");
    
    if (!compress_rle(&input, &compressed)) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    printf("Compression ratio: %.2f%%\n\n", 
           (double)(input.size - compressed.size) / input.size * 100);
    
    if (!decompress_rle(&compressed, &decompressed)) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(decompressed.data[i]);
    }
    printf("\n\n");
    
    printf("Decompressed data (hex):\n");
    print_buffer_hex(&decompressed);
    
    if (input.size != decompressed.size) {
        printf("Size mismatch error!\n");
        return 1;
    }
    
    if (memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Data mismatch error!\n");
        return 1;
    }
    
    printf("Compression and decompression successful.\n");
    return 0;
}