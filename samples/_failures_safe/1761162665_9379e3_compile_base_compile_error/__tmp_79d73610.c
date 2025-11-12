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

int validate_input(const uint8_t* input, size_t size) {
    if (input == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int process_compression(const uint8_t* input, size_t input_size, Buffer* result) {
    if (!validate_input(input, input_size)) return 0;
    
    size_t compressed_size = simple_compress(input, input_size, result->data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) return 0;
    
    result->size = compressed_size;
    return 1;
}

int process_decompression(const uint8_t* input, size_t input_size, Buffer* result) {
    if (!validate_input(input, input_size)) return 0;
    
    size_t decompressed_size = simple_decompress(input, input_size, result->data, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0) return 0;
    
    result->size = decompressed_size;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if (i < buffer->size - 1) printf(" ");
    }
    printf("\n");
}

int get_user_input(uint8_t* buffer, size_t max_size, size_t* actual_size) {
    char input[MAX_INPUT_SIZE * 3];
    
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len % 2 != 0) return 0;
    
    size_t bytes = len / 2;
    if (bytes > max_size) return 0;
    
    for (size_t i = 0; i < bytes; i++) {
        char hex[3] = {input[i * 2], input[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) return 0;
        buffer[i] = (uint8_t)value;
    }
    
    *actual_size = bytes;
    return 1;
}

int main(void) {
    printf("Compression/Decompression Tool\n");
    printf("Enter mode (1=compress, 2=decompress): ");
    
    int mode;
    if (scanf("%d", &mode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    if (mode != 1 && mode != 2) {
        printf("Invalid mode\n");
        return 1;
    }
    
    printf("Enter data as hex bytes (e.g., 414141): ");
    
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size;
    
    if (!get_user_input(input_data, MAX_INPUT_SIZE, &input_size)) {
        printf("Invalid input data\n");
        return 1;
    }
    
    Buffer result = {0};
    int success = 0;
    
    if (mode == 1) {
        success = process_compression(input_data, input_size, &result);
        if (success) {
            printf("Compressed data: ");
            print_buffer_hex(&result);
        } else {
            printf("Compression failed\n");
        }
    } else {