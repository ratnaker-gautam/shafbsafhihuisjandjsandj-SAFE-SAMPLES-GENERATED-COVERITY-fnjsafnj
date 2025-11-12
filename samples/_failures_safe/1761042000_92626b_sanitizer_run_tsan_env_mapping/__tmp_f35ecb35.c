//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: compression_stub
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

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
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
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        i += count;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        if (i + 1 >= input->length) break;
        
        uint8_t count = input->data[i++];
        uint8_t value = input->data[i++];
        
        for (uint8_t c = 0; c < count; c++) {
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output->data[out_idx++] = value;
        }
    }
    
    output->length = out_idx;
}

int buffers_equal(const Buffer* a, const Buffer* b) {
    if (a == NULL || b == NULL) return 0;
    if (a->length != b->length) return 0;
    
    for (size_t i = 0; i < a->length; i++) {
        if (a->data[i] != b->data[i]) return 0;
    }
    return 1;
}

int main(void) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    original.length = input_len;
    for (size_t i = 0; i < input_len; i++) {
        original.data[i] = (uint8_t)input_line[i];
    }
    
    if (!validate_input(original.data, original.length)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    simple_compress(&original, &compressed);
    simple_decompress(&compressed, &decompressed);
    
    if (!buffers_equal(&original, &decompressed)) {
        fprintf(stderr, "Compression/decompression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", original.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.length / original.length * 100.0f);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.length; i++) {
        if (isprint(compressed.data[i])) {
            printf("%c", compressed.data[i]);
        } else {
            printf("[%d]", compressed.data[i]);
        }
    }
    printf("\n");
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.length; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    return 0;
}