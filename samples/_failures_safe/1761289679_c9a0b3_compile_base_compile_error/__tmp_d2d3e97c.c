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
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    *output_size = out_idx;
    return 1;
}

int decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL || output_size == NULL) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    *output_size = out_idx;
    return 1;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int get_user_input(uint8_t* buffer, size_t* size) {
    if (buffer == NULL || size == NULL) return 0;
    
    printf("Enter input data (hex characters, max %d bytes): ", MAX_INPUT_SIZE / 2);
    
    char input[MAX_INPUT_SIZE * 2 + 1];
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len % 2 != 0 || len == 0) return 0;
    
    *size = len / 2;
    if (*size > MAX_INPUT_SIZE) return 0;
    
    for (size_t i = 0; i < *size; i++) {
        char hex[3] = {input[i * 2], input[i * 2 + 1], '\0'};
        
        if (!isxdigit(hex[0]) || !isxdigit(hex[1])) return 0;
        
        char* endptr;
        unsigned long value = strtoul(hex, &endptr, 16);
        if (endptr == hex || *endptr != '\0' || value > 255) return 0;
        
        buffer[i] = (uint8_t)value;
    }
    
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    size_t input_size = 0;
    size_t compressed_size = 0;
    size_t decompressed_size = 0;
    
    if (!get_user_input(input_data, &input_size)) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    print_buffer_hex(input_data, input_size);
    
    if (!compress_rle(input_data, input_size, compressed, &compressed_size)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_buffer_hex(compressed, compressed_size);
    
    if (!decompress_rle(compressed, compressed_size, decompressed, &decompressed_size)) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);