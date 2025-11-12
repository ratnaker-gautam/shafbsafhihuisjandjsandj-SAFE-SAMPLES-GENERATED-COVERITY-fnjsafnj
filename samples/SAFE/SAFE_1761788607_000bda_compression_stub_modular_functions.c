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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (output_capacity < input_size * 2) return 0;
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_index + 2 > output_capacity) return 0;
        
        output[output_index++] = count;
        output[output_index++] = current;
        i += count;
    }
    
    return output_index;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_capacity) {
    if (input_size % 2 != 0) return 0;
    
    size_t output_index = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (output_index + count > output_capacity) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_index++] = value;
        }
    }
    
    return output_index;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

int process_compression_test(void) {
    uint8_t test_input[] = "AAAABBBCCDAA";
    size_t input_size = strlen((char*)test_input);
    
    if (!validate_input(test_input, input_size)) {
        return 1;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = simple_compress(test_input, input_size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0) {
        return 1;
    }
    
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    size_t decompressed_size = simple_decompress(compressed, compressed_size, decompressed, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0) {
        return 1;
    }
    
    if (decompressed_size != input_size) {
        return 1;
    }
    
    if (!compare_buffers(test_input, decompressed, input_size)) {
        return 1;
    }
    
    printf("Compression test successful\n");
    printf("Original size: %zu, Compressed size: %zu\n", input_size, compressed_size);
    
    return 0;
}

int main(void) {
    return process_compression_test();
}