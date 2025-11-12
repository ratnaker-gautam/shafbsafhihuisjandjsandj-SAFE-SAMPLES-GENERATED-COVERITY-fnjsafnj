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
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_max - 3) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3) {
            output[out_idx++] = 0xFF;
            output[out_idx++] = current;
            output[out_idx++] = (uint8_t)count;
            i += count;
        } else {
            for (size_t j = 0; j < count && out_idx < output_max; j++) {
                output[out_idx++] = current;
            }
            i += count;
        }
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_len && out_idx < output_max) {
        if (i + 2 < input_len && input[i] == 0xFF) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (out_idx + count > output_max) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            i += 3;
        } else {
            output[out_idx++] = input[i++];
        }
    }
    
    return out_idx;
}

int compress_data(const char* input, size_t input_len, Buffer* compressed) {
    if (!validate_input(input, input_len) || compressed == NULL) return 0;
    
    size_t result_size = simple_compress((const uint8_t*)input, input_len, 
                                        compressed->data, MAX_OUTPUT_SIZE);
    
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) return 0;
    
    compressed->size = result_size;
    return 1;
}

int decompress_data(const Buffer* compressed, char* output, size_t output_max) {
    if (compressed == NULL || output == NULL || output_max == 0) return 0;
    if (compressed->size == 0 || compressed->size > MAX_OUTPUT_SIZE) return 0;
    
    size_t result_size = simple_decompress(compressed->data, compressed->size,
                                          (uint8_t*)output, output_max);
    
    if (result_size == 0 || result_size > output_max) return 0;
    
    output[result_size] = '\0';
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main(void) {
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
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!compress_data(input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size