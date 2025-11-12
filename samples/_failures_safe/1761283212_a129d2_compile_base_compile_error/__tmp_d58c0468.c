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

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size - 2) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 >= output_size) break;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        in_idx += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size) {
        if (in_idx + 1 >= input_size) break;
        
        uint8_t value = input[in_idx++];
        uint8_t count = input[in_idx++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t i = 0; i < count && out_idx < output_size; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char input[MAX_INPUT_SIZE * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    size_t data_size = 0;
    for (size_t i = 0; i < len && data_size < MAX_INPUT_SIZE; i++) {
        if (isprint((unsigned char)input[i])) {
            buffer->data[data_size++] = (uint8_t)input[i];
        }
    }
    
    if (data_size == 0) return 0;
    
    buffer->size = data_size;
    return 1;
}

void display_compression_stats(size_t original_size, size_t compressed_size) {
    if (original_size == 0) return;
    
    printf("Original size: %zu bytes\n", original_size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (compressed_size > 0) {
        double ratio = (double)original_size / compressed_size;
        printf("Compression ratio: %.2f:1\n", ratio);
    }
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed_data = {0};
    Buffer decompressed_data = {0};
    
    if (!get_user_input(&input_data)) {
        printf("Error: Invalid input or no data provided.\n");
        return 1;
    }
    
    printf("Processing %zu bytes of data...\n", input_data.size);
    
    compressed_data.size = compress_rle(input_data.data, input_data.size, 
                                      compressed_data.data, MAX_OUTPUT_SIZE);
    
    if (compressed_data.size == 0) {
        printf("Error: Compression failed.\n");
        return 1;
    }
    
    printf("Compression completed.\n");
    display_compression_stats(input_data.size, compressed_data.size);
    
    decompressed_data.size = decompress_rle(compressed_data.data, compressed_data.size,
                                           decompressed_data.data, MAX_INPUT_SIZE);
    
    if (decompressed_data.size == 0) {
        printf("Error: Decompression failed.\n");
        return 1;
    }
    
    if (decompressed_data.size == input_data.size && 
        memcmp(input_data.data, decompressed_data.data, input_data.size) == 0) {
        printf("Verification: Compression/decompression successful.\n");
    } else {
        printf("Error: Data verification failed.\n");
        return 1;
    }
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_data.size &&