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
        
        output[out_idx++] = count;
        output[out_idx++] = current;
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
        uint8_t count = input[in_idx++];
        uint8_t value = input[in_idx++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char temp[MAX_INPUT_SIZE + 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    memcpy(buffer->data, temp, len);
    buffer->size = len;
    return 1;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed_data = {0};
    Buffer decompressed_data = {0};
    
    if (!get_user_input(&input_data)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_data.size);
    print_buffer_hex(input_data.data, input_data.size);
    
    compressed_data.size = compress_rle(input_data.data, input_data.size, 
                                      compressed_data.data, MAX_OUTPUT_SIZE);
    
    if (compressed_data.size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_data.size);
    print_buffer_hex(compressed_data.data, compressed_data.size);
    
    decompressed_data.size = decompress_rle(compressed_data.data, compressed_data.size,
                                           decompressed_data.data, MAX_INPUT_SIZE);
    
    if (decompressed_data.size == 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_data.size);
    print_buffer_hex(decompressed_data.data, decompressed_data.size);
    
    if (decompressed_data.size == input_data.size && 
        memcmp(decompressed_data.data, input_data.data, input_data.size) == 0) {
        printf("Verification: Success - data matches original\n");
    } else {
        printf("Verification: Failed - data mismatch\n");
        return 1;
    }
    
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed_data.size / input_data.size) * 100.0);
    
    return 0;
}