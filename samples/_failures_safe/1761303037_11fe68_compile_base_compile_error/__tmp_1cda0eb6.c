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
        
        uint8_t value = input[in_idx];
        uint8_t count = input[in_idx + 1];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t i = 0; i < count && out_idx < output_size; i++) {
            output[out_idx++] = value;
        }
        
        in_idx += 2;
    }
    
    return out_idx;
}

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char temp[MAX_INPUT_SIZE * 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) return 0;
    
    size_t data_len = 0;
    for (size_t i = 0; i < len && data_len < MAX_INPUT_SIZE; i++) {
        if (isprint((unsigned char)temp[i])) {
            buffer->data[data_len++] = (uint8_t)temp[i];
        }
    }
    
    if (data_len == 0) return 0;
    
    buffer->size = data_len;
    return 1;
}

void display_hex(const uint8_t* data, size_t size) {
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
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_data.size);
    display_hex(input_data.data, input_data.size);
    
    size_t comp_size = compress_rle(input_data.data, input_data.size, 
                                   compressed.data, MAX_OUTPUT_SIZE);
    
    if (comp_size == 0) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    compressed.size = comp_size;
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    display_hex(compressed.data, compressed.size);
    
    size_t decomp_size = decompress_rle(compressed.data, compressed.size,
                                       decompressed.data, MAX_INPUT_SIZE);
    
    if (decomp_size == 0) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    decompressed.size = decomp_size;
    printf("\nDecompressed data (%zu bytes):\n", decompressed.size);
    display_hex(decompressed.data, decompressed.size);
    
    if (decompressed.size == input_data.size && 
        memcmp(decompressed.data, input_data.data, input_data.size) == 0) {
        printf("\nVerification: Data matches original\n");