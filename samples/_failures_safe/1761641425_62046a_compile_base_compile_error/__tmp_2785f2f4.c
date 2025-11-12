//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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

void simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input_size == 0) {
        *output_size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 3 >= MAX_OUTPUT_SIZE) break;
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (out_idx >= MAX_OUTPUT_SIZE) break;
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    *output_size = out_idx;
}

void simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count > MAX_INPUT_SIZE) break;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            i += 3;
        } else {
            if (out_idx >= MAX_INPUT_SIZE) break;
            output[out_idx++] = input[i++];
        }
    }
    
    *output_size = out_idx;
}

int buffers_equal(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

int main(void) {
    Buffer original, compressed, decompressed;
    uint8_t comp_buf[MAX_OUTPUT_SIZE];
    uint8_t decomp_buf[MAX_INPUT_SIZE];
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    original.size = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL && original.size < MAX_INPUT_SIZE) {
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
        
        unsigned int byte_val;
        if (sscanf(token, "%02x", &byte_val) != 1) {
            fprintf(stderr, "Failed to parse hex: %s\n", token);
            return 1;
        }
        
        original.data[original.size++] = (uint8_t)byte_val;
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(original.data, original.size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    size_t comp_size;
    simple_compress(original.data, original.size, comp_buf, &comp_size);
    
    size_t decomp_size;
    simple_decompress(comp_buf, comp_size, decomp_buf, &decomp_size);
    
    if (decomp_size != original.size || !buffers_equal(original.data, decomp_buf, original.size)) {
        fprintf(stderr, "Compression/decompression verification failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", original.size);
    printf("Compressed size: %zu bytes\n", comp_size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)comp_size / original.size) * 100.0);
    
    printf("Original data: ");
    for (size_t i = 0; i < original.size; i++) {