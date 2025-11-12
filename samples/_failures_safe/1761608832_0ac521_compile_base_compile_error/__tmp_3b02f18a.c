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

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || compress == NULL) return result;
    if (input_size > MAX_INPUT_SIZE) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t compressed_size = compress(input, input_size, output, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        free(output);
        return result;
    }
    
    result.data = malloc(compressed_size);
    if (result.data == NULL) {
        free(output);
        return result;
    }
    
    memcpy(result.data, output, compressed_size);
    result.size = compressed_size;
    free(output);
    
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || decompress == NULL) return result;
    if (input_size > MAX_OUTPUT_SIZE) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t decompressed_size = decompress(input, input_size, output, MAX_OUTPUT_SIZE);
    
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        free(output);
        return result;
    }
    
    result.data = malloc(decompressed_size);
    if (result.data == NULL) {
        free(output);
        return result;
    }
    
    memcpy(result.data, output, decompressed_size);
    result.size = decompressed_size;
    free(output);
    
    return result;
}

void print_buffer_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

void free_buffer(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    uint8_t test_data[] = {0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43};
    size_t test_size = sizeof(test_data);
    
    printf("Original data (%zu bytes): ", test_size);
    print_buffer_hex(test_data, test_size);
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    Buffer compressed = compress_data(test_data, test_size, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }