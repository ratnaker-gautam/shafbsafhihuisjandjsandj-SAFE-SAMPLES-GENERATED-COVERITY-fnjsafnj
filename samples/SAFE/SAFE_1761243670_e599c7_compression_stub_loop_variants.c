//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

void compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            break;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        size_t j = i + 1;
        
        for (; j < input_size && j < i + 255; j++) {
            if (input[j] == current) {
                count++;
            } else {
                break;
            }
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        
        i += count;
    }
}

void decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return;
    }
    
    output->size = 0;
    size_t i = 0;
    
    do {
        if (i + 1 >= input_size) {
            break;
        }
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            break;
        }
        
        size_t k = 0;
        while (k < count) {
            output->data[output->size + k] = value;
            k++;
        }
        output->size += count;
        
        i += 2;
    } while (i < input_size);
}

int main() {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        original.data[i] = (uint8_t)input_line[i];
    }
    original.size = input_len;
    
    compress_run_length(original.data, original.size, &compressed);
    decompress_run_length(compressed.data, compressed.size, &decompressed);
    
    printf("Original size: %zu bytes\n", original.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (compressed.size * 100.0) / (original.size > 0 ? original.size : 1));
    
    printf("Original: ");
    for (size_t i = 0; i < original.size; i++) {
        putchar(original.data[i]);
    }
    printf("\n");
    
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(decompressed.data[i]);
    }
    printf("\n");
    
    if (original.size == decompressed.size && 
        memcmp(original.data, decompressed.data, original.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    return 0;
}