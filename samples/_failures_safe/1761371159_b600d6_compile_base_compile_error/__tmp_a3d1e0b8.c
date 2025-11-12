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
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len && output_len + 2 <= output_max) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[output_len++] = (uint8_t)count;
        output[output_len++] = current;
        i += count;
    }
    
    return output_len;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL) return 0;
    if (input_len == 0 || output_max == 0) return 0;
    if (input_len % 2 != 0) return 0;
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len && output_len < output_max) {
        if (i + 1 >= input_len) break;
        
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (output_len + count > output_max) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_len++] = value;
        }
    }
    
    return output_len;
}

int compress_data(const char* input, size_t input_len, Buffer* compressed) {
    if (!validate_input(input, input_len)) return 0;
    if (compressed == NULL) return 0;
    
    size_t result_len = simple_compress((const uint8_t*)input, input_len, 
                                      compressed->data, MAX_OUTPUT_SIZE);
    
    if (result_len == 0) return 0;
    
    compressed->size = result_len;
    return 1;
}

int decompress_data(const Buffer* compressed, char* output, size_t output_max) {
    if (compressed == NULL || output == NULL) return 0;
    if (compressed->size == 0 || output_max == 0) return 0;
    
    size_t result_len = simple_decompress(compressed->data, compressed->size,
                                        (uint8_t*)output, output_max);
    
    if (result_len == 0) return 0;
    
    if (result_len < output_max) {
        output[result_len] = '\0';
    }
    
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main() {
    char input[MAX_INPUT_SIZE];
    char output[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", input_len);
    
    if (!compress_data(input, input_len, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data (hex):\n");
    print_buffer_hex(&compressed);
    
    if (!decompress_data(&compressed, output, sizeof(output))) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed: %s\n", output);
    
    size_t output_len = strlen(output);
    if (input_len == output_len && memcmp(input, output, input_len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    double ratio = (double)compressed.size