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

int compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return -1;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return -1;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_size && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 > MAX_INPUT_SIZE) return -1;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    *output_size = out_idx;
    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return -1;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE || input_size % 2 != 0) return -1;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (count == 0) return -1;
        
        if (out_idx + count > MAX_INPUT_SIZE) return -1;
        
        size_t k = 0;
        do {
            output[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    *output_size = out_idx;
    return 0;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main() {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    temp[strcspn(temp, "\n")] = 0;
    
    size_t len = strlen(temp);
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    memcpy(input.data, temp, len);
    input.size = len;
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer_hex(input.data, input.size);
    
    if (compress_rle(input.data, input.size, compressed.data, &compressed.size) != 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    printf("Compression ratio: %.2f%%\n", 
           (float)(input.size - compressed.size) / input.size * 100.0f);
    
    if (decompress_rle(compressed.data, compressed.size, decompressed.data, &decompressed.size) != 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(decompressed.data, decompressed.size);
    
    if (input.size != decompressed.size || memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Data mismatch after decompression!\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    
    return 0;
}