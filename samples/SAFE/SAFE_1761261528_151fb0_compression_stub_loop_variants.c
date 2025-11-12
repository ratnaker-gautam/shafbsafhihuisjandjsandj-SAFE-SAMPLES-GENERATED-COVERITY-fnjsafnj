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
        
        if (output->length + 2 <= MAX_OUTPUT_SIZE) {
            output->data[output->length++] = (uint8_t)count;
            output->data[output->length++] = current;
        } else {
            break;
        }
        
        i += count;
    }
}

void simple_decompress(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->length = 0;
    
    for (i = 0; i < input->length; i += 2) {
        if (i + 1 >= input->length) break;
        
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        size_t j = 0;
        do {
            if (output->length < MAX_OUTPUT_SIZE) {
                output->data[output->length++] = value;
            } else {
                break;
            }
            j++;
        } while (j < count);
        
        if (output->length >= MAX_OUTPUT_SIZE) break;
    }
}

int main(void) {
    Buffer original, compressed, decompressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
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
    
    printf("Original length: %zu\n", original.length);
    printf("Compressed length: %zu\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.length / original.length) * 100.0);
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.length; i++) {
        if (isprint(decompressed.data[i])) {
            putchar(decompressed.data[i]);
        } else {
            putchar('?');
        }
    }
    printf("\n");
    
    int match = 1;
    if (original.length != decompressed.length) {
        match = 0;
    } else {
        for (size_t i = 0; i < original.length; i++) {
            if (original.data[i] != decompressed.data[i]) {
                match = 0;
                break;
            }
        }
    }
    
    printf("Verification: %s\n", match ? "PASS" : "FAIL");
    
    return match ? 0 : 1;
}