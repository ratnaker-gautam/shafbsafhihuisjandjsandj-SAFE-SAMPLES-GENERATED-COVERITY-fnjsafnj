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

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* compressed) {
    if (!validate_input(input, input_size)) return 0;
    
    size_t result_size = simple_compress(input, input_size, compressed->data, MAX_OUTPUT_SIZE);
    if (result_size == 0) return 0;
    
    compressed->size = result_size;
    return 1;
}

int decompress_data(const uint8_t* input, size_t input_size, Buffer* decompressed) {
    if (!validate_input(input, input_size)) return 0;
    
    size_t result_size = simple_decompress(input, input_size, decompressed->data, MAX_OUTPUT_SIZE);
    if (result_size == 0) return 0;
    
    decompressed->size = result_size;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 2 == 0) printf(" ");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main(void) {
    uint8_t test_input[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t input_size = sizeof(test_input);
    
    printf("Original data (%zu bytes):\n", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", test_input[i]);
    }
    printf("\n\n");
    
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    if (!compress_data(test_input, input_size, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    printf("\n");
    
    if (!decompress_data(compressed.data, compressed.size, &decompressed)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%02X ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size != input_size) {
        printf("Size mismatch: %zu vs %zu\n", decompressed.size, input_size);
        return 1;
    }
    
    if (memcmp(test_input, decompressed.data, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}