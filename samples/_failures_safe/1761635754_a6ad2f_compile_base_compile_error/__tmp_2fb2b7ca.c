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

int compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
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
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < buffer->size) printf(" ");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

int get_user_input(uint8_t* buffer, size_t* size) {
    char input[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter hex data (max %d bytes): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0) return 0;
    
    *size = input_len / 2;
    if (*size > MAX_INPUT_SIZE) return 0;
    
    for (size_t i = 0; i < *size; i++) {
        char hex_byte[3] = {input[i * 2], input[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) return 0;
        buffer[i] = (uint8_t)value;
    }
    
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    Buffer compressed, decompressed;
    
    printf("Run-Length Compression/Decompression\n");
    
    if (!get_user_input(input_data, &input_size)) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_size);
    Buffer original = { .data = {0}, .size = input_size };
    memcpy(original.data, input_data, input_size);
    print_buffer_hex(&original);
    
    printf("\nCompressing...\n");
    if (!compress_run_length(input_data, input_size, &compressed)) {
        printf("Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    printf("\nDecompressing...\n");
    if (!decompress_run_length(compressed.data, compressed.size, &decompressed)) {
        printf("Error: Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (decompressed.size == original.size && 
        memcmp(decompressed.data, original.data, original.size) == 0) {
        printf("\nVerification: SUCCESS - Data matches original\n");
    } else {
        printf("\nVerification: FAILED - Data does not match original\n");