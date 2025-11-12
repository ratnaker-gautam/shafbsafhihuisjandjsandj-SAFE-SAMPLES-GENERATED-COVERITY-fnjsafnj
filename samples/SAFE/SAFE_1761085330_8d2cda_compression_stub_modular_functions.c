//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

int compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    if (!validate_input(input, input_size)) return 0;
    if (output == NULL) return 0;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output->data[output->size] = count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    
    return 1;
}

int decompress_run_length(const Buffer* input, uint8_t* output, size_t* output_size) {
    if (input == NULL || output == NULL || output_size == NULL) return 0;
    if (input->size == 0 || input->size % 2 != 0) return 0;
    
    size_t decompressed_size = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (decompressed_size + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[decompressed_size++] = value;
        }
    }
    
    *output_size = decompressed_size;
    return 1;
}

int compare_buffers(const uint8_t* buf1, size_t size1, const uint8_t* buf2, size_t size2) {
    if (buf1 == NULL || buf2 == NULL) return 0;
    if (size1 != size2) return 0;
    
    for (size_t i = 0; i < size1; i++) {
        if (buf1[i] != buf2[i]) return 0;
    }
    
    return 1;
}

void print_buffer_hex(const uint8_t* buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", buffer[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    uint8_t test_input[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t input_size = sizeof(test_input);
    
    printf("Original data: ");
    print_buffer_hex(test_input, input_size);
    
    Buffer compressed;
    if (!compress_run_length(test_input, input_size, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    print_buffer_hex(compressed.data, compressed.size);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size;
    
    if (!decompress_run_length(&compressed, decompressed, &decompressed_size)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data: ");
    print_buffer_hex(decompressed, decompressed_size);
    
    if (compare_buffers(test_input, input_size, decompressed, decompressed_size)) {
        printf("Compression/decompression successful\n");
    } else {
        printf("Compression/decompression failed\n");
        return 1;
    }
    
    return 0;
}