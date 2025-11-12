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
    output->length = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (output->length + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        output->data[output->length++] = (uint8_t)count;
        output->data[output->length++] = current;
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->length = 0;
    
    for (; i < input->length; i += 2) {
        if (i + 1 >= input->length) {
            break;
        }
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (output->length + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        do {
            output->data[output->length++] = value;
            k++;
        } while (k < count);
    }
}

int main() {
    Buffer original, compressed, decompressed;
    
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
    
    if (!validate_input((uint8_t*)input_line, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    original.length = input_len;
    memcpy(original.data, input_line, input_len);
    
    simple_compress(&original, &compressed);
    simple_decompress(&compressed, &decompressed);
    
    printf("Original length: %zu\n", original.length);
    printf("Compressed length: %zu\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.length / original.length) * 100);
    
    int match = 1;
    size_t idx = 0;
    for (; idx < original.length; idx++) {
        if (idx >= decompressed.length || original.data[idx] != decompressed.data[idx]) {
            match = 0;
            break;
        }
    }
    
    if (match && original.length == decompressed.length) {
        printf("Decompression verified: SUCCESS\n");
    } else {
        printf("Decompression verified: FAILED\n");
        return 1;
    }
    
    return 0;
}