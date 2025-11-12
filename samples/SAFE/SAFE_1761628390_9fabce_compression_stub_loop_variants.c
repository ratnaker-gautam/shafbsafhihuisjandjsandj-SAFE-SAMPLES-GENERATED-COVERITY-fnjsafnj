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

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i += count;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    size_t out_idx = 0;
    
    while (i < input->length) {
        if (i + 1 >= input->length) {
            output->length = 0;
            return;
        }
        
        uint8_t value = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) {
            output->length = 0;
            return;
        }
        
        for (uint8_t c = 0; c < count; c++) {
            output->data[out_idx++] = value;
        }
        
        i += 2;
    }
    
    output->length = out_idx;
}

int compare_buffers(const Buffer* a, const Buffer* b) {
    if (a->length != b->length) {
        return 0;
    }
    
    for (size_t idx = 0; idx < a->length; idx++) {
        if (a->data[idx] != b->data[idx]) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int ch;
    size_t len = 0;
    while ((ch = getchar()) != EOF && ch != '\n' && len < MAX_INPUT_SIZE) {
        original.data[len++] = (uint8_t)ch;
    }
    original.length = len;
    
    if (!validate_input(original.data, original.length)) {
        printf("Invalid input\n");
        return 1;
    }
    
    simple_compress(&original, &compressed);
    
    if (compressed.length == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", original.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    
    if (compressed.length < original.length) {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.length / original.length) * 100.0);
    } else {
        printf("No compression achieved\n");
    }
    
    simple_decompress(&compressed, &decompressed);
    
    if (decompressed.length == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (compare_buffers(&original, &decompressed)) {
        printf("Round-trip verification: SUCCESS\n");
    } else {
        printf("Round-trip verification: FAILED\n");
        return 1;
    }
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.length; i++) {
        if (isprint(compressed.data[i])) {
            printf("%c", compressed.data[i]);
        } else {
            printf("[0x%02X]", compressed.data[i]);
        }
    }
    printf("\n");
    
    return 0;
}