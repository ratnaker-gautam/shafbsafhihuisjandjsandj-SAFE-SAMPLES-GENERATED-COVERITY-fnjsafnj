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
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, CompressedData* output) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size && out_index < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            output->data[out_index++] = 0xFF;
            output->data[out_index++] = current;
            output->data[out_index++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && out_index < MAX_OUTPUT_SIZE; j++) {
                output->data[out_index++] = current;
            }
        }
        
        i += count;
    }
    
    output->size = out_index;
    return out_index;
}

size_t simple_decompress(const CompressedData* compressed, uint8_t* output, size_t max_output_size) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < compressed->size && out_index < max_output_size) {
        if (compressed->data[i] == 0xFF && i + 2 < compressed->size) {
            uint8_t value = compressed->data[i + 1];
            uint8_t count = compressed->data[i + 2];
            
            if (out_index + count > max_output_size) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_index++] = value;
            }
            
            i += 3;
        } else {
            output[out_index++] = compressed->data[i++];
        }
    }
    
    return out_index;
}

int compare_data(const uint8_t* data1, const uint8_t* data2, size_t size) {
    if (data1 == NULL || data2 == NULL) return 0;
    for (size_t i = 0; i < size; i++) {
        if (data1[i] != data2[i]) return 0;
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
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    CompressedData compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char buffer[MAX_INPUT_SIZE];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_length = strlen(buffer);
    if (input_length > 0 && buffer[input_length - 1] == '\n') {
        buffer[input_length - 1] = '\0';
        input_length--;
    }
    
    if (!validate_input(buffer, input_length)) {
        printf("Invalid input\n");
        return 1;
    }
    
    memcpy(input_data, buffer, input_length);
    
    printf("\nOriginal data (%zu bytes):\n", input_length);
    print_hex(input_data, input_length);
    
    size_t compressed_size = simple_compress(input_data, input_length, &compressed);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    size_t decompressed_size = simple_decompress(&compressed, decompressed_data, sizeof(decompressed_data));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed_data, decompressed_size);
    
    if (compare_data(input_data, decompressed_data, input_length)) {
        printf("\nCompression/Decompression successful!\n");
        if (input_length > 0