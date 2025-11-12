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
        
        for (uint8_t j = 0; j < count && out_idx < MAX_OUTPUT_SIZE; j++) {
            output->data[out_idx++] = value;
        }
    }
    
    output->size = out_idx;
    return 0;
}

int decompress_copy(const Buffer *input, Buffer *output) {
    return compress_copy(input, output);
}

void print_buffer(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_OUTPUT_SIZE];
    
    Buffer input = {input_data, 0};
    Buffer compressed = {compressed_data, 0};
    Buffer decompressed = {decompressed_data, 0};
    
    printf("Enter data size (1-%zu): ", MAX_INPUT_SIZE);
    int size_input;
    if (scanf("%d", &size_input) != 1 || size_input <= 0 || size_input > (int)MAX_INPUT_SIZE) {
        printf("Invalid size\n");
        return 1;
    }
    
    input.size = (size_t)size_input;
    
    printf("Enter %zu bytes as hex values (00 FF etc): ", input.size);
    for (size_t i = 0; i < input.size; i++) {
        unsigned int byte;
        if (scanf("%02x", &byte) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        input.data[i] = (uint8_t)byte;
    }
    
    CompressionFunc compress_funcs[] = {compress_rle, compress_copy};
    CompressionFunc decompress_funcs[] = {decompress_rle, decompress_copy};
    const char *names[] = {"RLE", "Copy"};
    
    printf("\nOriginal data:\n");
    print_buffer(&input);
    
    for (int i = 0; i < 2; i++) {
        memset(compressed_data, 0, MAX_OUTPUT_SIZE);
        memset(decompressed_data, 0, MAX_OUTPUT_SIZE);
        compressed.size = 0;
        decompressed.size = 0;
        
        printf("\n%s Compression:\n", names[i]);
        
        if (compress_funcs[i](&input, &compressed) == 0) {
            printf("Compressed (%zu bytes):\n", compressed.size);
            print_buffer(&compressed);
            
            if (decompress_funcs[i](&compressed, &decompressed) == 0) {
                printf("Decompressed (%zu bytes):\n", decompressed.size);
                print_buffer(&decompressed);
                
                if (decompressed.size == input.size && 
                    memcmp(decompressed.data, input.data, input.size) ==