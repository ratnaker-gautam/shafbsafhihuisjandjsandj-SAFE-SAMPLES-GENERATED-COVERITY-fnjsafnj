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

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (output_size < input_size * 2) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (out_idx + 2 > output_size) return 0;
        
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 1 >= input_size) return 0;
        
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

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    if (input->size > MAX_INPUT_SIZE) return 0;
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) return 0;
    
    size_t result_size = compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (result_size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL) return 0;
    if (input->data == NULL || input->size == 0) return 0;
    if (input->size > MAX_OUTPUT_SIZE) return 0;
    
    output->data = malloc(MAX_INPUT_SIZE);
    if (output->data == NULL) return 0;
    
    size_t result_size = decompress(input->data, input->size, output->data, MAX_INPUT_SIZE);
    if (result_size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

void print_buffer(const Buffer *buffer) {
    if (buffer == NULL || buffer->data == NULL) return;
    
    for (size_t i = 0; i < buffer->size; i++) {
        printf("%02X ", buffer->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buffer->size % 16 != 0) printf("\n");
}

void cleanup_buffer(Buffer *buffer) {
    if (buffer != NULL && buffer->data != NULL) {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
    }
}

int main() {
    Buffer original = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    original.data = test_data;
    original.size = sizeof(test_data);
    
    printf("Original data (%zu bytes):\n", original.size);
    print_buffer(&original);
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    if (compress_data(compress_func, &original, &compressed)) {
        printf("Compressed data (%zu bytes):\n", compressed.size);
        print_buffer(&compressed);
        
        if (decompress_data(decompress_func, &compressed, &decompressed)) {
            printf("De