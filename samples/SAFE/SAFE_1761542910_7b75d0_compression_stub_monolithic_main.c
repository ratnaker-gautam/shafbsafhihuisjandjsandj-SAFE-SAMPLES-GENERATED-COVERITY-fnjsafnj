//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
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
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
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
            output->data[output->size] = value;
            output->size++;
        }
    }
    
    return 0;
}

int main() {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    size_t pos = 0;
    while ((c = getchar()) != EOF && c != '\n' && pos < MAX_INPUT_SIZE - 1) {
        input_data.data[pos] = (uint8_t)c;
        pos++;
    }
    input_data.size = pos;
    
    if (input_data.size == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_data.size);
    
    if (compress_rle(input_data.data, input_data.size, &compressed) != 0) {
        printf("Compression failed: output buffer too small\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / (double)input_data.size) * 100.0);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    if (decompressed.size != input_data.size || 
        memcmp(decompressed.data, input_data.data, input_data.size) != 0) {
        printf("Verification failed: decompressed data doesn't match original\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    printf("Original: ");
    for (size_t i = 0; i < input_data.size && i < 50; i++) {
        if (isprint(input_data.data[i])) {
            putchar(input_data.data[i]);
        } else {
            printf("\\x%02X", input_data.data[i]);
        }
    }
    if (input_data.size > 50) printf("...");
    printf("\n");
    
    return 0;
}