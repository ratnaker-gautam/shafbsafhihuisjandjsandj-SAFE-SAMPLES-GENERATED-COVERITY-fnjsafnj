//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char temp[MAX_INPUT_SIZE * 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) return 0;
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    buffer->size = len;
    memcpy(buffer->data, temp, len);
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!get_user_input(&input_data)) {
        printf("Error: Failed to read input\n");
        return 1;
    }
    
    if (!validate_input(input_data.data, input_data.size)) {
        printf("Error: Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_data.size);
    print_hex(input_data.data, input_data.size);
    
    compressed.size = simple_compress(input_data.data, input_data.size, 
                                    compressed.data, sizeof(compressed.data));
    
    if (compressed.size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    decompressed.size = simple_decompress(compressed.data, compressed.size,
                                        decompressed.data, sizeof(decompressed.data));
    
    if (decompressed.size == 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed.size);
    print_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size != input_data.size || 
        memcmp(input_data.data, decompressed.data, input_data.size) != 0) {
        printf("Error: Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Success: Compression and decompression verified\n");
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_data.size) * 100.0);
    
    return 0;
}