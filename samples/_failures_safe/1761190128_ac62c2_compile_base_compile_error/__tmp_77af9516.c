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
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (count >= 3) {
            if (out_idx + 2 >= output_size) break;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            in_idx += count;
        } else {
            if (out_idx + count >= output_size) break;
            for (size_t i = 0; i < count; i++) {
                output[out_idx++] = current;
            }
            in_idx += count;
        }
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size && out_idx < output_size) {
        if (in_idx + 1 < input_size && input[in_idx] == input[in_idx + 1]) {
            uint8_t value = input[in_idx];
            size_t count = 1;
            
            while (in_idx + count < input_size && count < 255 && input[in_idx + count] == value) {
                count++;
            }
            
            if (count >= 3) {
                if (out_idx + count > output_size) break;
                for (size_t i = 0; i < count; i++) {
                    output[out_idx++] = value;
                }
                in_idx += count;
            } else {
                if (out_idx + count > output_size) break;
                for (size_t i = 0; i < count; i++) {
                    output[out_idx++] = value;
                }
                in_idx += count;
            }
        } else {
            if (out_idx >= output_size) break;
            output[out_idx++] = input[in_idx++];
        }
    }
    
    return out_idx;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data to compress (max %d bytes as hex): ", MAX_INPUT_SIZE);
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[hex_len - 1] = '\0';
        hex_len--;
    }
    
    if (hex_len % 2 != 0) {
        printf("Invalid hex input: must have even number of characters\n");
        return 1;
    }
    
    size_t data_size = hex_len / 2;
    if (data_size > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            printf("Invalid hex byte: %s\n", hex_byte);
            return 1;
        }
        input.data[i] = (uint8_t)value;
    }
    input.size = data_size;
    
    if (!validate_input