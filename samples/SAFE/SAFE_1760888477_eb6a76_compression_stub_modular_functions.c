//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_run_length(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (!validate_input(input, input_size) || output == NULL) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_index + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[out_index++] = current;
        output->data[out_index++] = (uint8_t)count;
        i += count;
    }
    
    output->size = out_index;
    return 1;
}

int decompress_run_length(const CompressedData* compressed, uint8_t* output, size_t* output_size) {
    if (compressed == NULL || output == NULL || output_size == NULL) return 0;
    if (compressed->size == 0 || compressed->size % 2 != 0) return 0;
    
    size_t out_index = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        uint8_t value = compressed->data[i];
        uint8_t count = compressed->data[i + 1];
        
        if (out_index + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_index++] = value;
        }
    }
    
    *output_size = out_index;
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter input string (max %d characters): ", MAX_INPUT_SIZE);
    
    char buffer[MAX_INPUT_SIZE + 2];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    memcpy(input_data, buffer, input_len);
    
    printf("Original data (%zu bytes): ", input_len);
    print_hex(input_data, input_len);
    
    if (!compress_run_length(input_data, input_len, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    size_t decompressed_size;
    if (!decompress_run_length(&compressed, output_data, &decompressed_size)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed_size);
    print_hex(output_data, decompressed_size);
    
    if (decompressed_size == input_len && memcmp(input_data, output_data, input_len) == 0) {
        printf("Verification: SUCCESS\n");
        float ratio = (float)compressed.size / input_len * 100.0f;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}