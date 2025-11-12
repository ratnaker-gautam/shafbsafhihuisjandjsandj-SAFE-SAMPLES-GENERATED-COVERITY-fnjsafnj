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
        
        output[output_len++] = count;
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
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (output_len + count > output_max) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[output_len++] = value;
        }
    }
    
    return output_len;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t len) {
    if (buf1 == NULL || buf2 == NULL) return 0;
    for (size_t i = 0; i < len; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t len) {
    if (buffer == NULL || len == 0) return;
    for (size_t i = 0; i < len; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    const char* test_string = "AAAABBBCCDAA";
    size_t input_len = strlen(test_string);
    
    if (input_len >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(input_data, test_string, input_len);
    
    if (!validate_input((const char*)input_data, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", test_string);
    printf("Original hex: ");
    print_buffer_hex(input_data, input_len);
    
    size_t compressed_len = simple_compress(input_data, input_len, compressed_data, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed (%zu bytes): ", compressed_len);
    print_buffer_hex(compressed_data, compressed_len);
    
    size_t decompressed_len = simple_decompress(compressed_data, compressed_len, decompressed_data, MAX_INPUT_SIZE);
    if (decompressed_len == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed (%zu bytes): ", decompressed_len);
    print_buffer_hex(decompressed_data, decompressed_len);
    
    if (decompressed_len != input_len) {
        fprintf(stderr, "Size mismatch: original %zu, decompressed %zu\n", input_len, decompressed_len);
        return 1;
    }
    
    if (!compare_buffers(input_data, decompressed_data, input_len)) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful!\n");
    
    double compression_ratio = (double)compressed_len / input_len;
    printf("Compression ratio: %.2f\n", compression_ratio);
    
    return 0;
}