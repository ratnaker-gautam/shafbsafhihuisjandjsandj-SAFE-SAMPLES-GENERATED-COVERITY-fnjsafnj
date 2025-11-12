//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
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

void compress_rle(const Buffer* input, Buffer* output) {
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input->size) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        size_t j = in_idx + 1;
        while (j < input->size && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_INPUT_SIZE) {
            break;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
}

void decompress_rle(const Buffer* input, Buffer* output) {
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input->size) {
        if (in_idx + 1 >= input->size) {
            break;
        }
        
        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];
        
        if (out_idx + count > MAX_INPUT_SIZE) {
            break;
        }
        
        for (uint8_t i = 0; i < count; i++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (buf->size % 16 != 0) {
        printf("\n");
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
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input_str, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    original.size = len;
    for (size_t i = 0; i < len; i++) {
        original.data[i] = (uint8_t)input_str[i];
    }
    
    compressed.size = 0;
    decompressed.size = 0;
    memset(compressed.data, 0, sizeof(compressed.data));
    memset(decompressed.data, 0, sizeof(decompressed.data));
    
    compress_rle(&original, &compressed);
    decompress_rle(&compressed, &decompressed);
    
    printf("\nOriginal size: %zu bytes\n", original.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / original.size) * 100.0);
    
    printf("\nCompressed data (hex):\n");
    print_buffer_hex(&compressed);
    
    printf("\nDecompressed text: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(decompressed.data[i]);
    }
    printf("\n");
    
    if (memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("Decompression successful: data matches original\n");
    } else {
        printf("Decompression failed: data mismatch\n");
        return 1;
    }
    
    return 0;
}