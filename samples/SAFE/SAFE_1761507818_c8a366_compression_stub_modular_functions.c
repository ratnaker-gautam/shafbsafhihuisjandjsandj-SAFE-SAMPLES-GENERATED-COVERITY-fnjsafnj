//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_rle(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input->size == 0 || input->size % 2 != 0) return 0;
    
    size_t decompressed_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (decompressed_size + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_size++] = value;
        }
    }
    
    *output_size = decompressed_size;
    return 1;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

int main() {
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    Buffer compressed;
    size_t decompressed_size;
    
    printf("Enter data to compress (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \n");
    size_t input_size = 0;
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte_val;
        if (sscanf(token, "%02X", &byte_val) != 1) {
            printf("Invalid hex input\n");
            return 1;
        }
        original_data[input_size++] = (uint8_t)byte_val;
        token = strtok(NULL, " \n");
    }
    
    if (input_size == 0) {
        printf("No valid input provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    print_buffer_hex(original_data, input_size);
    
    if (!compress_rle(original_data, input_size, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    if (!decompress_rle(&compressed, decompressed_data, &decompressed_size)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    print_buffer_hex(decompressed_data, decompressed_size);
    
    if (decompressed_size != input_size || memcmp(original_data, decompressed_data, input_size) != 0) {
        printf("Verification failed: decompressed data doesn't match original\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_size) * 100.0);
    
    return 0;
}