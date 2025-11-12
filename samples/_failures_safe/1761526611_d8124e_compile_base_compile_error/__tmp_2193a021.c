//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_OUTPUT_SIZE - 2; ) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && 
               input[i + count] == current && out_idx < MAX_OUTPUT_SIZE - 2) {
            count++;
        }
        
        if (count >= 3) {
            if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
            output.data[out_idx++] = 0xFF;
            output.data[out_idx++] = current;
            output.data[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            for (size_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output.data[out_idx++] = current;
                i++;
            }
        }
    }
    
    output.size = out_idx;
    return output;
}

Buffer decompress_data(const uint8_t* input, size_t input_size) {
    Buffer output = {0};
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input_size && out_idx < MAX_OUTPUT_SIZE; ) {
        if (i + 2 < input_size && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count == 0 || out_idx + count > MAX_OUTPUT_SIZE) {
                break;
            }
            
            for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
                output.data[out_idx++] = value;
            }
            i += 3;
        } else {
            if (out_idx >= MAX_OUTPUT_SIZE) break;
            output.data[out_idx++] = input[i];
            i++;
        }
    }
    
    output.size = out_idx;
    return output;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    char input_str[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter data to compress (hex string): ");
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0 || input_len > MAX_INPUT_SIZE * 2) {
        printf("Invalid input: must be even-length hex string\n");
        return 1;
    }
    
    size_t data_size = input_len / 2;
    if (data_size > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {input_str[i * 2], input_str[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) {
            printf("Invalid hex byte: %s\n", hex_byte);
            return 1;
        }
        input_data[i] = (uint8_t)value;
    }
    
    Buffer compressed = compress_data(input_data, data_size);
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    if (decompressed.size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%02X", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size != data_size) {
        printf("Size mismatch: original %zu, decompressed %zu\n", data_size, decompressed.size);
        return 1;
    }
    
    if (memcmp(input_data, decompressed.data, data_size) != 0) {