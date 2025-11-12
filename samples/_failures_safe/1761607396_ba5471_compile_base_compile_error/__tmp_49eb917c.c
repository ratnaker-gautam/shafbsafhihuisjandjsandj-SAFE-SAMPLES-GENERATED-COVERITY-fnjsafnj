//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef int (*CompressionFunc)(const Buffer *input, Buffer *output);

int compress_rle(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) return -1;
    if (input->size == 0) return -1;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx + 2 < MAX_OUTPUT_SIZE) {
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
    
    if (in_idx < input->size) return -1;
    output->size = out_idx;
    return 0;
}

int compress_copy(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) return -1;
    if (input->size > MAX_OUTPUT_SIZE) return -1;
    
    for (size_t i = 0; i < input->size && i < MAX_OUTPUT_SIZE; i++) {
        output->data[i] = input->data[i];
    }
    output->size = input->size;
    return 0;
}

int decompress_rle(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) return -1;
    if (input->size % 2 != 0) return -1;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size && out_idx < MAX_OUTPUT_SIZE; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) return -1;
        
        for (uint8_t j = 0; j < count; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 0;
}

int decompress_copy(const Buffer *input, Buffer *output) {
    return compress_copy(input, output);
}

int process_compression(CompressionFunc compress_func, CompressionFunc decompress_func) {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer compressed = {compressed_data, 0};
    Buffer decompressed = {decompressed_data, 0};
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1) return -1;
    if (size_input <= 0 || size_input > (int)MAX_INPUT_SIZE) return -1;
    
    input.size = (size_t)size_input;
    
    printf("Enter %zu bytes (0-255): ", input.size);
    for (size_t i = 0; i < input.size; i++) {
        int byte_input;
        if (scanf("%d", &byte_input) != 1) return -1;
        if (byte_input < 0 || byte_input > 255) return -1;
        input.data[i] = (uint8_t)byte_input;
    }
    
    if (compress_func(&input, &compressed) != 0) return -1;
    
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    if (decompress_func(&compressed, &decompressed) != 0) return -1;
    
    if (decompressed.size != input.size) return -1;
    
    int match = 1;
    for (size_t i = 0; i < input.size; i++) {
        if (input.data[i] != decompressed.data[i]) {
            match = 0;
            break;
        }
    }
    
    if (match) {
        printf("Decompression successful: data matches original\n");
    } else {
        printf("Decompression failed: data mismatch\n");
        return -1;
    }
    
    return 0;
}

int main() {
    CompressionFunc compress_funcs[] = {compress_rle, compress_copy};
    CompressionFunc decompress_funcs[] = {decompress_rle, decompress_copy};
    const char *names[] = {"RLE", "Copy"};
    size_t num_methods = sizeof(names) / sizeof(names[0]);
    
    printf("Available compression methods:\n");
    for (size_t i = 0; i < num_methods; i++) {
        printf("%zu: %s\n", i, names[i