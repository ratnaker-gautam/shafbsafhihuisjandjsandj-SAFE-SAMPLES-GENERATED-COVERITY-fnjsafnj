//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_INPUT_SIZE) return 0;
    return 1;
}

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (!validate_input(input->data, input->size)) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size) {
        if (out_idx + 2 > MAX_OUTPUT_SIZE) return 0;
        
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }
    
    output->size = out_idx;
    return 1;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL) return 0;
    if (input->size == 0 || input->size > MAX_OUTPUT_SIZE) return 0;
    if (input->size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_INPUT_SIZE) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int get_user_input(Buffer* buf) {
    if (buf == NULL) return 0;
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    int size;
    if (scanf("%d", &size) != 1) return 0;
    if (size <= 0 || size > (int)MAX_INPUT_SIZE) return 0;
    
    printf("Enter %d bytes as hex values (space separated): ", size);
    for (int i = 0; i < size; i++) {
        unsigned int value;
        if (scanf("%02X", &value) != 1) return 0;
        if (value > 255) return 0;
        buf->data[i] = (uint8_t)value;
    }
    
    buf->size = (size_t)size;
    return 1;
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("RLE Compression Demo\n");
    
    if (!get_user_input(&input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original data: ");
    print_buffer_hex(&input);
    
    if (!compress_rle(&input, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    print_buffer_hex(&compressed);
    printf("Compression ratio: %.2f\n", 
           (float)input.size / (compressed.size > 0 ? (float)compressed.size : 1.0f));
    
    if (!decompress_rle(&compressed, &decompressed)) {
        printf("Decompression failed\n");
        return 1;
    }
    
    printf("Decompressed data: ");
    print_buffer_hex(&decompressed);
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    return 0;
}