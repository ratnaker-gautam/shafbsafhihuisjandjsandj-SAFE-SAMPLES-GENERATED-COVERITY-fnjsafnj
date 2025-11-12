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

size_t compress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t decompress_rle(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int process_compression(const uint8_t* input, size_t input_size, Buffer* result) {
    if (!validate_input(input, input_size)) return 0;
    
    size_t compressed_size = compress_rle(input, input_size, result->data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) return 0;
    
    result->size = compressed_size;
    return 1;
}

int process_decompression(const uint8_t* input, size_t input_size, Buffer* result) {
    if (!validate_input(input, input_size)) return 0;
    
    size_t decompressed_size = decompress_rle(input, input_size, result->data, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0) return 0;
    
    result->size = decompressed_size;
    return 1;
}

void print_buffer(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int get_user_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    printf("Enter input data (hex bytes, space separated): ");
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    size_t idx = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL && idx < max_size) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) return 0;
        if (byte > 255) return 0;
        buffer[idx++] = (uint8_t)byte;
        token = strtok(NULL, " \t\n");
    }
    
    *actual_size = idx;
    return idx > 0;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size;
    Buffer output;
    
    printf("Compression/Decompression Tool\n");
    printf("1. Compress data\n");
    printf("2. Decompress data\n");
    printf("Choose operation: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    if (!get_user_input(input, MAX_INPUT_SIZE, &input_size)) {
        printf("Failed to read input\n");
        return 1;
    }
    
    int success = 0;
    if (choice == 1) {
        success = process_compression(input, input_size, &output);
        if (success) {
            printf("Compressed data (%zu bytes): ", output.size);
            print_buffer(&output);
        }
    } else if (choice == 2) {
        success = process_decompression(input, input_size, &output);
        if (success) {
            printf("Decompressed data (%zu bytes): ", output.size);
            print_buffer(&output);
        }
    } else {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (!success) {
        printf("Operation failed\n");
        return 1;
    }
    
    return 0;
}