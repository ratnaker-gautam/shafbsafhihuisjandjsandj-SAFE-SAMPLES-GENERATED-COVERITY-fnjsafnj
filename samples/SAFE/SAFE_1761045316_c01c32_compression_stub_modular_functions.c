//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < UINT8_MAX && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = current;
        output->data[output->size + 1] = (uint8_t)count;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    if (input_size % 2 != 0) return 0;
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size] = value;
            output->size++;
        }
    }
    
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if (i < buffer->size - 1) printf(" ");
    }
    printf("\n");
}

int read_user_input(Buffer* input) {
    if (input == NULL) return 0;
    
    printf("Enter input data (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    input->size = 0;
    char line[4096];
    
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    char* token = strtok(line, " \n");
    while (token != NULL && input->size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) return 0;
        if (byte > 0xFF) return 0;
        
        input->data[input->size] = (uint8_t)byte;
        input->size++;
        token = strtok(NULL, " \n");
    }
    
    return 1;
}

int main(void) {
    Buffer input_data;
    Buffer compressed;
    Buffer decompressed;
    
    printf("Run-Length Compression Demo\n");
    
    if (!read_user_input(&input_data)) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (input_data.size == 0) {
        printf("Error: No input data provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_data.size);
    print_buffer_hex(&input_data);
    
    if (!compress_run_length(input_data.data, input_data.size, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(&compressed);
    
    if (!decompress_run_length(compressed.data, compressed.size, &decompressed)) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (input_data.size != decompressed.size) {
        printf("Error: Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(input_data.data, decompressed.data, input_data.size) != 0) {
        printf("Error: Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression successful. Ratio: %.2f%%\n", 
           (float)compressed.size / input_data.size * 100.0f);
    
    return 0;
}