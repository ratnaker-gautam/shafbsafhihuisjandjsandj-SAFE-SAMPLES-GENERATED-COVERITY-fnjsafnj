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
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = (uint8_t)count;
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
        uint8_t count = input[i];
        uint8_t value = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
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

void print_buffer_hex(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 2 == 0) printf(" ");
    }
    if (buffer->size > 0) printf("\n");
}

int compare_buffers(const Buffer* a, const Buffer* b) {
    if (a->size != b->size) return 0;
    return memcmp(a->data, b->data, a->size) == 0;
}

int main(void) {
    uint8_t test_input[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    const char* test_string = "AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJ";
    size_t input_size = strlen(test_string);
    
    if (input_size >= MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(test_input, test_string, input_size);
    
    printf("Original data (%zu bytes):\n", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%c", test_input[i]);
    }
    printf("\n\n");
    
    if (!compress_data(test_input, input_size, &compressed)) {
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
    printf("\n\n");
    
    if (compare_buffers(&(Buffer){.data = test_input, .size = input_size}, &decompressed)) {
        printf("Verification: SUCCESS - Original and decompressed data match\n");
    } else {
        printf("Verification: FAILED - Data mismatch\n");
        return 1;
    }
    
    return 0;
}