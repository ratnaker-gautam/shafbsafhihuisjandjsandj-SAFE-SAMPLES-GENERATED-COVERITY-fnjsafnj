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
    
    for (size_t i = 0; i < input->size; i++) {
        output->data[i] = input->data[i];
    }
    
    output->size = input->size;
    return 0;
}

int decompress_rle(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->data == NULL) return -1;
    if (input->size % 2 != 0) return -1;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->size; i += 2) {
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

int process_compression(CompressionFunc compress, CompressionFunc decompress, 
                       const Buffer *input, Buffer *output) {
    if (compress == NULL || decompress == NULL) return -1;
    
    Buffer temp = {0};
    temp.data = malloc(MAX_OUTPUT_SIZE);
    if (temp.data == NULL) return -1;
    
    int result = compress(input, &temp);
    if (result != 0) {
        free(temp.data);
        return -1;
    }
    
    result = decompress(&temp, output);
    free(temp.data);
    return result;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer output = {output_data, 0};
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input.size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input.size == 0 || input.size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", input.size);
    for (size_t i = 0; i < input.size; i++) {
        unsigned int value;
        if (scanf("%u", &value) != 1) {
            printf("Invalid input byte\n");
            return 1;
        }
        if (value > 255) {
            printf("Byte value out of range\n");
            return 1;
        }
        input.data[i] = (uint8_t)value;
    }
    
    CompressionFunc compressors[] = {compress_rle, compress_copy};
    CompressionFunc decompressors[] = {decompress_rle, decompress_copy};
    const char *names[] = {"RLE", "Copy"};
    
    for (int i = 0; i < 2; i++) {
        printf("\nTesting %s compression:\n", names[i]);
        
        int result = process_compression(compressors[i], decompressors[i], 
                                       &input, &output);
        if (result != 0) {
            printf("Compression/decompression failed\n");
            continue;
        }
        
        if (input.size != output.size) {
            printf("Size mismatch: %zu -> %zu\n", input.size, output.size);
            continue;
        }
        
        int match = 1;
        for (size_t j = 0; j < input.size; j++) {
            if (input.data[j] != output.data[j]) {
                match = 0;
                break;
            }
        }
        
        if (match) {
            printf("Success: input and output match\n");
        }