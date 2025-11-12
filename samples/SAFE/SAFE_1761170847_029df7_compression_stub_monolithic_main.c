//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return -1;
    }
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }
    }
    
    return 0;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && bytes_read < MAX_INPUT_SIZE) {
        input_data[bytes_read++] = (uint8_t)c;
    }
    
    if (bytes_read == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    if (bytes_read == MAX_INPUT_SIZE && c != '\n' && c != EOF) {
        printf("Input too large. Truncated to %d bytes.\n", MAX_INPUT_SIZE);
        while ((c = getchar()) != EOF && c != '\n');
    }
    
    printf("Original data size: %zu bytes\n", bytes_read);
    
    if (compress_rle(input_data, bytes_read, &compressed) != 0) {
        printf("Compression failed: output buffer too small\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (float)compressed.size / bytes_read * 100.0f);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (decompressed.size != bytes_read) {
        printf("Decompression size mismatch\n");
        return 1;
    }
    
    if (memcmp(input_data, decompressed.data, bytes_read) != 0) {
        printf("Decompression data mismatch\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    if (compressed.size > 32) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}