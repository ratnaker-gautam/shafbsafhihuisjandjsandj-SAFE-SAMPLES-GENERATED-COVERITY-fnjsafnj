//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} CompressedData;

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < length; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_index >= MAX_OUTPUT_SIZE - 2) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output->data[out_index++] = 0xFF;
            output->data[out_index++] = current;
            output->data[out_index++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                output->data[out_index++] = current;
            }
        }
        
        i += count;
    }
    
    output->size = out_index;
    return out_index;
}

size_t simple_decompress(const CompressedData* compressed, uint8_t* output, size_t output_size) {
    if (compressed == NULL || output == NULL) return 0;
    
    size_t in_index = 0;
    size_t out_index = 0;
    
    while (in_index < compressed->size) {
        if (out_index >= output_size) return 0;
        
        if (compressed->data[in_index] == 0xFF && in_index + 2 < compressed->size) {
            uint8_t value = compressed->data[in_index + 1];
            uint8_t count = compressed->data[in_index + 2];
            
            if (out_index + count > output_size) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_index++] = value;
            }
            
            in_index += 3;
        } else {
            output[out_index++] = compressed->data[in_index++];
        }
    }
    
    return out_index;
}

int verify_compression(const uint8_t* original, size_t original_size, const uint8_t* decompressed, size_t decompressed_size) {
    if (original == NULL || decompressed == NULL) return 0;
    if (original_size != decompressed_size) return 0;
    
    for (size_t i = 0; i < original_size; i++) {
        if (original[i] != decompressed[i]) return 0;
    }
    
    return 1;
}

void print_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_length = strlen(input_buffer);
    if (input_length > 0 && input_buffer[input_length - 1] == '\n') {
        input_buffer[input_length - 1] = '\0';
        input_length--;
    }
    
    if (!validate_input(input_buffer, input_length)) {
        printf("Invalid input\n");
        return 1;
    }
    
    memcpy(input_data, input_buffer, input_length);
    size_t input_size = input_length;
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Original data (hex):\n");
    print_hex(input_data, input_size);
    
    memset(&compressed, 0, sizeof(compressed));
    size_t compressed_size = simple_compress(input_data, input_size, &compressed);
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed_size / (float)input_size * 100.0f);
    printf("Compressed data (hex):\n");
    print_