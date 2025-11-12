//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
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

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    *output_size = out_idx;
    return 1;
}

int decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size == NULL) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    *output_size = out_idx;
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data to compress (max %d bytes as hex, space separated):\n", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input.size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        
        int valid = 1;
        for (int i = 0; i < 2; i++) {
            if (!isxdigit(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (!valid) {
            fprintf(stderr, "Invalid hex digit in: %s\n", token);
            return 1;
        }
        
        unsigned int byte;
        if (sscanf(token, "%02x", &byte) != 1) {
            fprintf(stderr, "Failed to parse hex: %s\n", token);
            return 1;
        }
        
        input.data[input.size++] = (uint8_t)byte;
        token = strtok(NULL, " \t\n");
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_hex(input.data, input.size);
    
    if (!compress_rle(input.data, input.size, compressed.data, &compressed.size)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)(input.size - compressed.size) / input.size * 100);
    
    if (!decompress_rle(compressed.data, compressed.size, decompressed.data, &decompressed.size)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size != input.size) {
        fprintf(stderr, "Size mismatch