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
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
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
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || compress_func == NULL) {
        return result;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_func(input, input_size, output, MAX_OUTPUT_SIZE);
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

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || decompress_func == NULL) {
        return result;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input, input_size, output, MAX_OUTPUT_SIZE);
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

void free_buffer(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 5, 5};
    size_t test_size = sizeof(test_data);
    
    printf("Original data: ");
    for (size_t i = 0; i < test_size; i++) {
        printf("%d ", test_data[i]);
    }
    printf("\n");
    
    CompressFunc compress = run_length_encode;
    DecompressFunc decompress = run_length_decode;
    
    Buffer compressed = compress_data(test_data, test_size, compress);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free_buffer(&compressed);
        return 1;
    }