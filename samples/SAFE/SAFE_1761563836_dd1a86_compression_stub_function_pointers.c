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

size_t dummy_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    
    return input_size;
}

size_t dummy_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    for (size_t i = 0; i < input_size && i < output_size; i++) {
        output[i] = input[i] ^ 0x55;
    }
    
    return input_size;
}

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || compress_func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t compressed_size = compress_func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = compressed_size;
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress_func) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || decompress_func == NULL) {
        return result;
    }
    
    uint8_t *output_buffer = malloc(MAX_OUTPUT_SIZE);
    if (output_buffer == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress_func(input, input_size, output_buffer, MAX_OUTPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size > MAX_OUTPUT_SIZE) {
        free(output_buffer);
        return result;
    }
    
    result.data = output_buffer;
    result.size = decompressed_size;
    return result;
}

int compare_buffers(const uint8_t *buf1, size_t size1, const uint8_t *buf2, size_t size2) {
    if (buf1 == NULL || buf2 == NULL || size1 != size2) {
        return 0;
    }
    
    for (size_t i = 0; i < size1; i++) {
        if (buf1[i] != buf2[i]) {
            return 0;
        }
    }
    
    return 1;
}

int main() {
    uint8_t test_data[] = "This is a test string for compression demonstration.";
    size_t test_size = sizeof(test_data) - 1;
    
    if (test_size == 0 || test_size > MAX_INPUT_SIZE) {
        printf("Invalid test data size\n");
        return 1;
    }
    
    CompressFunc compress = dummy_compress;
    DecompressFunc decompress = dummy_decompress;
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %zu bytes\n", test_size);
    
    Buffer compressed = compress_data(test_data, test_size, compress);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed data: %s\n", decompressed.data);
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (compare_buffers(test_data, test_size, decompressed.data, decompressed.size)) {
        printf("Round-trip successful: data matches original\n");
    } else {
        printf("Round-trip failed: data does not match original\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}