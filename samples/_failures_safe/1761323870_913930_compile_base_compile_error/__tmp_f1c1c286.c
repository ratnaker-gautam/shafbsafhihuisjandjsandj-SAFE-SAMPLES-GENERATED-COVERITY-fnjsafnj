//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx + 2 < output_max) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_len && count < 255 && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 1 || current >= 192) {
            if (out_idx + 2 >= output_max) break;
            output[out_idx++] = 192 + (count - 1);
            output[out_idx++] = current;
        } else {
            if (out_idx + 1 >= output_max) break;
            output[out_idx++] = current;
        }
        
        in_idx += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_len && out_idx < output_max) {
        uint8_t current = input[in_idx++];
        
        if (current >= 192 && in_idx < input_len) {
            size_t count = current - 192 + 1;
            uint8_t value = input[in_idx++];
            
            for (size_t i = 0; i < count && out_idx < output_max; i++) {
                output[out_idx++] = value;
            }
        } else {
            if (out_idx < output_max) {
                output[out_idx++] = current;
            }
        }
    }
    
    return out_idx;
}

int compress_data(const char* input, size_t input_len, Buffer* compressed) {
    if (!validate_input(input, input_len)) return 0;
    if (compressed == NULL) return 0;
    
    compressed->size = simple_compress((const uint8_t*)input, input_len, 
                                     compressed->data, MAX_OUTPUT_SIZE);
    return compressed->size > 0;
}

int decompress_data(const Buffer* compressed, char* output, size_t output_size) {
    if (compressed == NULL || output == NULL) return 0;
    if (output_size == 0) return 0;
    
    size_t decompressed_size = simple_decompress(compressed->data, compressed->size,
                                               (uint8_t*)output, output_size - 1);
    if (decompressed_size == 0) return 0;
    
    output[decompressed_size] = '\0';
    return 1;
}

void print_buffer_hex(const uint8_t* data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    char decompressed[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_data(input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data (hex):\n");
    print_buffer_hex(compressed.data, compressed.size);
    
    if (!decompress_data(&compressed, decompressed, sizeof(decompressed))) {
        fprintf(stderr, "Decompression failed