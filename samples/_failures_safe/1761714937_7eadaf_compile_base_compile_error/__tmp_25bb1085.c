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
    size_t length;
} Buffer;

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t len, uint8_t* output) {
    if (output == NULL) {
        return 0;
    }
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < len) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_pos + 3 >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[out_pos++] = 0xFF;
            output[out_pos++] = current;
            output[out_pos++] = (uint8_t)count;
        } else {
            if (out_pos + count >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            for (size_t j = 0; j < count; j++) {
                output[out_pos++] = current;
            }
        }
        
        i += count;
    }
    
    return out_pos;
}

size_t simple_decompress(const uint8_t* input, size_t len, uint8_t* output) {
    if (output == NULL) {
        return 0;
    }
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < len) {
        if (input[i] == 0xFF && i + 2 < len) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_pos + count >= MAX_INPUT_SIZE) {
                return 0;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_pos++] = value;
            }
            
            i += 3;
        } else {
            if (out_pos >= MAX_INPUT_SIZE) {
                return 0;
            }
            output[out_pos++] = input[i++];
        }
    }
    
    return out_pos;
}

void print_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (len > 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buffer = {0};
    Buffer compressed_buffer = {0};
    Buffer decompressed_buffer = {0};
    
    printf("Enter data to compress (hex characters, max %d bytes): ", MAX_INPUT_SIZE);
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0) {
        fprintf(stderr, "Invalid hex input: must have even number of characters\n");
        return 1;
    }
    
    input_buffer.length = hex_len / 2;
    if (input_buffer.length > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_buffer.length; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (endptr != hex_byte + 2 || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex character\n");
            return 1;
        }
        input_buffer.data[i] = (uint8_t)value;
    }
    
    if (!validate_input(input_buffer.data, input_buffer.length)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_buffer.length);
    print_hex(input_buffer.data, input_buffer.length);
    
    compressed_buffer.length = simple_compress(input_buffer.data, input_buffer.length, compressed_buffer.data);
    if (compressed_buffer.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_buffer.length);
    print_