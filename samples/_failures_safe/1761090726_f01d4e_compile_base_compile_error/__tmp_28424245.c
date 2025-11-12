//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > output_size) return 0;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (!validate_buffer(input) || output == NULL) return 0;
    if (compress == NULL) return 0;
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) return 0;
    
    size_t result_size = compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (result_size == 0 || result_size > MAX_OUTPUT_SIZE) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (!validate_buffer(input) || output == NULL) return 0;
    if (decompress == NULL) return 0;
    
    output->data = malloc(MAX_INPUT_SIZE);
    if (output->data == NULL) return 0;
    
    size_t result_size = decompress(input->data, input->size, output->data, MAX_INPUT_SIZE);
    if (result_size == 0 || result_size > MAX_INPUT_SIZE) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    output->size = result_size;
    return 1;
}

void print_buffer_hex(const Buffer *buf) {
    if (!validate_buffer(buf)) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main() {
    Buffer original, compressed, decompressed;
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    
    original.data = test_data;
    original.size = sizeof(test_data);
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Original data (%zu bytes):\n", original.size);
    print_buffer_hex(&original);
    
    if (!compress_data(compress_func, &original, &compressed)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    if (!decompress_data(decompress_func, &compressed, &decompressed)) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed data (%zu bytes):\n", decompressed.size);
    print_buffer_hex(&decompressed);
    
    if (decompressed.size == original.size && 
        memcmp(de