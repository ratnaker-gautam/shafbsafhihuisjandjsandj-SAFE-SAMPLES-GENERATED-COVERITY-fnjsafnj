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
    if (input == NULL || output == NULL) return 0;
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_idx + 2 > output_size) return 0;
            output[out_idx++] = 0xFF;
            output[out_idx++] = (uint8_t)count;
            output[out_idx++] = current;
        } else {
            if (out_idx + count > output_size) return 0;
            for (size_t j = 0; j < count; j++) {
                output[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    return out_idx;
}

size_t simple_decompress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (input[i] == 0xFF && i + 2 < input_size) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (out_idx + count > output_size) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                output[out_idx++] = value;
            }
            
            i += 3;
        } else {
            if (out_idx >= output_size) return 0;
            output[out_idx++] = input[i++];
        }
    }
    
    return out_idx;
}

int compress_data(const uint8_t* input, size_t input_size, Buffer* compressed) {
    if (compressed == NULL) return 0;
    if (!validate_input(input, input_size)) return 0;
    
    size_t result_size = simple_compress(input, input_size, compressed->data, MAX_OUTPUT_SIZE);
    if (result_size == 0) return 0;
    
    compressed->size = result_size;
    return 1;
}

int decompress_data(const uint8_t* input, size_t input_size, Buffer* decompressed) {
    if (decompressed == NULL) return 0;
    if (!validate_input(input, input_size)) return 0;
    
    size_t result_size = simple_decompress(input, input_size, decompressed->data, MAX_OUTPUT_SIZE);
    if (result_size == 0) return 0;
    
    decompressed->size = result_size;
    return 1;
}

void print_buffer_hex(const Buffer* buffer) {
    if (buffer == NULL) return;
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

int compare_buffers(const Buffer* a, const Buffer* b) {
    if (a == NULL || b == NULL) return 0;
    if (a->size != b->size) return 0;
    return memcmp(a->data, b->data, a->size) == 0;
}

int main(void) {
    uint8_t test_data[] = {0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x03, 0x04, 0x04, 0x04};
    size_t test_size = sizeof(test_data);
    
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Original data (%zu bytes):\n", test_size);
    for (size_t i = 0; i < test_size; i++) {
        printf("%02X ", test_data[i]);
    }
    printf("\n\n");
    
    if (!compress_data(test_data, test_size, &compressed)) {
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
    
    printf("Decompressed data (%zu bytes):\n", decompressed.size