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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        uint8_t current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input_size && count < 255 && input[in_idx + count] == current) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        in_idx += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input_size) {
        if (in_idx + 1 >= input_size) return 0;
        
        uint8_t count = input[in_idx++];
        uint8_t value = input[in_idx++];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t i = 0; i < count; i++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* compressed) {
    if (!validate_input(input, input_size)) return 0;
    if (compressed == NULL) return 0;
    
    size_t result_size = simple_compress(input, input_size, compressed->data, MAX_OUTPUT_SIZE);
    if (result_size == 0) return 0;
    
    compressed->size = result_size;
    return 1;
}

int decompress_data(const uint8_t* input, size_t input_size, Buffer* decompressed) {
    if (!validate_input(input, input_size)) return 0;
    if (decompressed == NULL) return 0;
    
    size_t result_size = simple_decompress(input, input_size, decompressed->data, MAX_OUTPUT_SIZE);
    if (result_size == 0) return 0;
    
    decompressed->size = result_size;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < buffer->size) printf(" ");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t test_input[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    const char* test_string = "AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJ";
    size_t test_len = strlen(test_string);
    
    if (test_len >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Test string too long\n");
        return 1;
    }
    
    memcpy(test_input, test_string, test_len);
    
    printf("Original data (%zu bytes):\n", test_len);
    for (size_t i = 0; i < test_len; i++) {
        printf("%c", test_input[i]);
    }
    printf("\n\n");
    
    if (!compress_data(test_input, test_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    printf("\n");
    
    if (!decompress_data(compressed.data, compressed.size, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%c", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size != test_len) {
        fprintf(stderr, "Size mismatch after decompression\n");
        return 1;
    }
    
    if (memcmp(test_input, decompressed.data, test_len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}