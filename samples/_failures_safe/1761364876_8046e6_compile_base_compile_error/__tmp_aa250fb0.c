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
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
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
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    *output_size = out_idx;
    return 1;
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

int get_user_input(Buffer* buffer) {
    if (buffer == NULL) return 0;
    
    printf("Enter data to compress (max %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE * 3];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
    
    char* token = strtok(line, " ");
    buffer->size = 0;
    
    while (token != NULL && buffer->size < MAX_INPUT_SIZE) {
        if (strlen(token) != 2) return 0;
        
        int valid = 1;
        for (int i = 0; i < 2; i++) {
            if (!isxdigit(token[i])) valid = 0;
        }
        if (!valid) return 0;
        
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) return 0;
        if (byte > 255) return 0;
        
        buffer->data[buffer->size++] = (uint8_t)byte;
        token = strtok(NULL, " ");
    }
    
    return buffer->size > 0;
}

int main(void) {
    Buffer input_data;
    uint8_t compressed[MAX_OUTPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t compressed_size, decompressed_size;
    
    if (!get_user_input(&input_data)) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_data.size);
    print_hex(input_data.data, input_data.size);
    
    if (!compress_rle(input_data.data, input_data.size, compressed, &compressed_size)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed, compressed_size);
    
    if (!decompress_rle(compressed, compressed_size, decompressed, &decompressed_size)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed, decompressed_size);
    
    if (decompressed_size != input_data.size || memcmp(input_data.data, decompressed, input_data.size) != 0) {
        printf("Verification failed: decompressed data doesn't match original\n");
        return