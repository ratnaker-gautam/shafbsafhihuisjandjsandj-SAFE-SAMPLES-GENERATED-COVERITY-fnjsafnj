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
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_idx + 2 > output_size) return 0;
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_idx++] = value;
        }
        
        i += 2;
    }
    
    return output_idx;
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
        printf("Compression ratio: %.2f%%\n", ratio);
    }
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    if (!get_user_input(&input_buffer)) {
        printf("Error: Failed to read input\n");
        return 1;
    }
    
    if (!validate_input(input_buffer.data, input_buffer.size)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    compressed_buffer.size = simple_compress(input_buffer.data, input_buffer.size, 
                                           compressed_buffer.data, sizeof(compressed_buffer.data));
    
    if (compressed_buffer.size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compression completed successfully\n");
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