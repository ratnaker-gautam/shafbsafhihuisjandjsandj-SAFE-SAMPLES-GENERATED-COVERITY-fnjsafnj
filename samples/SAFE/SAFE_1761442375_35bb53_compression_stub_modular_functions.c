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
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
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
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp[MAX_INPUT_SIZE];
    if (fgets(temp, sizeof(temp), stdin) == NULL) return 0;
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    memcpy(buffer->data, temp, len);
    buffer->size = len;
    return 1;
}

void display_compression_stats(size_t original_size, size_t compressed_size) {
    if (original_size == 0) return;
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size > 0) {
        double ratio = (double)compressed_size / original_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    if (!get_user_input(&input_buffer)) {
        printf("Error: Invalid input or empty string\n");
        return 1;
    }
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        printf("Error: Input validation failed\n");
        return 1;
    }
    
    compressed_buffer.size = simple_compress(input_buffer.data, input_buffer.size, 
                                           compressed_buffer.data, sizeof(compressed_buffer.data));
    
    if (compressed_buffer.size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compression successful\n");
    display_compression_stats(input_buffer.size, compressed_buffer.size);
    
    decompressed_buffer.size = simple_decompress(compressed_buffer.data, compressed_buffer.size,
                                                decompressed_buffer.data, sizeof(decompressed_buffer.data));
    
    if (decompressed_buffer.size == 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    if (decompressed_buffer.size != input_buffer.size || 
        memcmp(decompressed_buffer.data, input_buffer.data, input_buffer.size) != 0) {
        printf("Error: Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    printf("Original: %.*s\n", (int)input_buffer.size, input_buffer.data);
    printf("Decompressed: %.*s\n", (int)decompressed_buffer.size, decompressed_buffer.data);
    
    return 0;
}