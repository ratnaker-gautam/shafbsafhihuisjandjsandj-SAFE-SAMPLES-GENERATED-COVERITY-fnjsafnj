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

void simple_compress(const Buffer* input, Buffer* output) {
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
        
        if (count > 3 || current == '\\' || current == '\'') {
            if (out_idx + 3 >= MAX_INPUT_SIZE) break;
            output->data[out_idx++] = '\\';
            output->data[out_idx++] = (uint8_t)count;
            output->data[out_idx++] = current;
        } else {
            for (size_t k = 0; k < count; k++) {
                if (out_idx >= MAX_INPUT_SIZE) break;
                output->data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output->size = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    size_t in_idx = 0;
    size_t out_idx = 0;
    
    while (in_idx < input->size) {
        if (input->data[in_idx] == '\\' && in_idx + 2 < input->size) {
            uint8_t count = input->data[in_idx + 1];
            uint8_t value = input->data[in_idx + 2];
            
            if (out_idx + count > MAX_INPUT_SIZE) break;
            
            for (uint8_t i = 0; i < count; i++) {
                output->data[out_idx++] = value;
            }
            in_idx += 3;
        } else {
            if (out_idx >= MAX_INPUT_SIZE) break;
            output->data[out_idx++] = input->data[in_idx++];
        }
    }
    
    output->size = out_idx;
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
    
    if (!validate_input(input_line, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(original.data, input_line, input_len);
    original.size = input_len;
    
    simple_compress(&original, &compressed);
    
    printf("Original size: %zu\n", original.size);
    printf("Compressed size: %zu\n", compressed.size);
    
    if (compressed.size < original.size) {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed.size / original.size) * 100.0);
    } else {
        printf("No compression achieved\n");
    }
    
    simple_decompress(&compressed, &decompressed);
    
    if (decompressed.size == original.size && 
        memcmp(decompressed.data, original.data, original.size) == 0) {
        printf("Decompression successful\n");
    } else {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        if (isprint(compressed.data[i]) || isspace(compressed.data[i])) {
            putchar(compressed.data[i]);
        } else {
            printf("\\x%02X", compressed.data[i]);
        }
    }
    printf("\n");
    
    return 0;
}