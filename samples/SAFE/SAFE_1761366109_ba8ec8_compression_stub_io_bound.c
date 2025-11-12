//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
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
        if (output->size + input[i] > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        for (size_t j = 0; j < input[i]; j++) {
            output->data[output->size++] = input[i + 1];
        }
    }
    
    return 0;
}

int main() {
    Buffer input_buf = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n' && bytes_read < MAX_INPUT_SIZE) {
        input_buf.data[bytes_read++] = (uint8_t)c;
    }
    input_buf.size = bytes_read;
    
    if (input_buf.size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_buf.size);
    
    if (compress_rle(input_buf.data, input_buf.size, &compressed) != 0) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_buf.size) * 100);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed.\n");
        return 1;
    }
    
    if (decompressed.size != input_buf.size || 
        memcmp(decompressed.data, input_buf.data, input_buf.size) != 0) {
        printf("Verification failed: decompressed data doesn't match original.\n");
        return 1;
    }
    
    printf("Verification successful: data integrity maintained.\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 32) {
        printf("...");
    }
    printf("\n");
    
    return 0;
}