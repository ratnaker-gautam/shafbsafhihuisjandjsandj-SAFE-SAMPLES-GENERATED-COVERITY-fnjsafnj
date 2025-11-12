//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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
    if (input_size == 0) return 0;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_index + 2 >= MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[out_index++] = (uint8_t)count;
        output->data[out_index++] = current;
        i += count;
    }
    
    output->size = out_index;
    return out_index;
}

size_t simple_decompress(const CompressedData* compressed, uint8_t* output, size_t max_output_size) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0) return 0;
    
    size_t out_index = 0;
    
    for (size_t i = 0; i < compressed->size; i += 2) {
        if (i + 1 >= compressed->size) return 0;
        
        uint8_t count = compressed->data[i];
        uint8_t value = compressed->data[i + 1];
        
        if (out_index + count > max_output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_index++] = value;
        }
    }
    
    return out_index;
}

int compare_data(const uint8_t* data1, const uint8_t* data2, size_t length) {
    if (data1 == NULL || data2 == NULL) return 0;
    for (size_t i = 0; i < length; i++) {
        if (data1[i] != data2[i]) return 0;
    }
    return 1;
}

void print_hex(const uint8_t* data, size_t length) {
    if (data == NULL || length == 0) return;
    for (size_t i = 0; i < length; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 2 == 0) printf(" ");
    }
    if (length % 16 != 0) printf("\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    uint8_t original_data[MAX_INPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
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
    
    memcpy(original_data, input_buffer, input_length);
    
    printf("\nOriginal data (%zu bytes):\n", input_length);
    print_hex(original_data, input_length);
    
    size_t compressed_size = simple_compress(original_data, input_length, &compressed);
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_size);
    print_hex(compressed.data, compressed_size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (float)compressed_size / input_length * 100.0f);
    
    size_t decompressed_size = simple_decompress(&compressed, decompressed_data, sizeof(decompressed_data));
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("\nDecompressed data (%zu bytes):\n", decompressed_size);
    print_hex(decompressed_data, decompressed_size);
    
    if (decompressed_size == input_length && 
        compare_data(original_data, decompressed_data, input_length)) {
        printf("\nVerification: SUCCESS - Data matches original\n");
    } else {
        printf("\nVerification: FAILED - Data does not match original\n");
        return 1;
    }
    
    printf("\nDecompressed text: ");
    for (size_t