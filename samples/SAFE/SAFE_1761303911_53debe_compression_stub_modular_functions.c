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
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 1 >= input_size) return 0;
        
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
    }
    
    return out_idx;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
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

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    if (!get_user_input(&input_buffer)) {
        printf("Error: Failed to read input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buffer.size);
    print_hex(input_buffer.data, input_buffer.size);
    printf("Text: %s\n\n", input_buffer.data);
    
    size_t compressed_size = simple_compress(input_buffer.data, input_buffer.size, 
                                           compressed_buffer.data, sizeof(compressed_buffer.data));
    
    if (compressed_size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed_buffer.data, compressed_size);
    
    size_t decompressed_size = simple_decompress(compressed_buffer.data, compressed_size,
                                               decompressed_buffer.data, sizeof(decompressed_buffer.data));
    
    if (decompressed_size == 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed_buffer.data, decompressed_size);
    printf("Text: %s\n", decompressed_buffer.data);
    
    if (decompressed_size == input_buffer.size && 
        memcmp(input_buffer.data, decompressed_buffer.data, input_buffer.size) == 0) {
        printf("\nSuccess: Data verified correctly\n");
        double ratio = (double)compressed_size / input_buffer.size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("\nError: Data verification failed\n");
        return 1;
    }
    
    return 0;
}