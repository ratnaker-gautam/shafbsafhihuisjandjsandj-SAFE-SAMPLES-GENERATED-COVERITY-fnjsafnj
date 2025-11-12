//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const uint8_t *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL) return;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) break;
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i += count;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL) return;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    for (; i < input->length; i += 2) {
        if (i + 1 >= input->length) break;
        
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) break;
        
        size_t k = 0;
        do {
            output->data[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    output->length = out_idx;
}

int main(void) {
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input((uint8_t*)temp_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(original.data, temp_input, input_len);
    original.length = input_len;
    
    memset(compressed.data, 0, sizeof(compressed.data));
    compressed.length = 0;
    
    memset(decompressed.data, 0, sizeof(decompressed.data));
    decompressed.length = 0;
    
    simple_compress(&original, &compressed);
    simple_decompress(&compressed, &decompressed);
    
    printf("Original length: %zu\n", original.length);
    printf("Compressed length: %zu\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           (compressed.length * 100.0) / (original.length > 0 ? original.length : 1));
    
    if (original.length == decompressed.length && 
        memcmp(original.data, decompressed.data, original.length) == 0) {
        printf("Decompression successful: data matches original\n");
    } else {
        printf("Decompression failed: data mismatch\n");
        return 1;
    }
    
    printf("Compressed data (hex): ");
    size_t idx = 0;
    while (idx < compressed.length) {
        printf("%02X ", compressed.data[idx]);
        idx++;
    }
    printf("\n");
    
    return 0;
}