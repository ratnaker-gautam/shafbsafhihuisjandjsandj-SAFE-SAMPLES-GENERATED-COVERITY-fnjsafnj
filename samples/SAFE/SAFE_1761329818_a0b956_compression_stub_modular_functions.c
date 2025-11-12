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
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    if (input_size % 2 != 0) return 0;
    
    output->size = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output->size + count > MAX_OUTPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size] = value;
            output->size++;
        }
    }
    
    return 1;
}

void print_buffer(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
    }
    printf("\n");
}

int get_user_input(uint8_t* buffer, size_t max_size) {
    if (buffer == NULL) return 0;
    
    printf("Enter input data (hex bytes, space separated): ");
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    size_t byte_count = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL && byte_count < max_size) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) return 0;
        if (byte > 0xFF) return 0;
        
        buffer[byte_count] = (uint8_t)byte;
        byte_count++;
        token = strtok(NULL, " \t\n");
    }
    
    return (int)byte_count;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    int input_size = get_user_input(input_data, MAX_INPUT_SIZE);
    
    if (input_size <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    Buffer compressed;
    Buffer decompressed;
    
    printf("Original data: ");
    Buffer original = { .size = (size_t)input_size };
    memcpy(original.data, input_data, (size_t)input_size);
    print_buffer(&original);
    
    if (compress_rle(input_data, (size_t)input_size, &compressed)) {
        printf("Compressed data: ");
        print_buffer(&compressed);
        
        if (decompress_rle(compressed.data, compressed.size, &decompressed)) {
            printf("Decompressed data: ");
            print_buffer(&decompressed);
            
            if (decompressed.size == original.size && 
                memcmp(decompressed.data, original.data, original.size) == 0) {
                printf("Verification: SUCCESS\n");
            } else {
                printf("Verification: FAILED\n");
            }
        } else {
            printf("Decompression failed\n");
        }
    } else {
        printf("Compression failed\n");
    }
    
    return 0;
}