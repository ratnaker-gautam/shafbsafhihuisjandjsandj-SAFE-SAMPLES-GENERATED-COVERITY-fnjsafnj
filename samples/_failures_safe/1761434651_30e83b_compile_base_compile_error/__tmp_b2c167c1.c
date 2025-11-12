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

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    memcpy(output, input, input_size);
    return input_size;
}

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < 2) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0 || output_size < input_size / 2 * 255) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        uint8_t value = input[i++];
        uint8_t count = input[i++];
        
        for (uint8_t j = 0; j < count && out_idx < output_size; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer process_data(const uint8_t *input, size_t input_size, CompressFunc compress, DecompressFunc decompress) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || compress == NULL || decompress == NULL) {
        return result;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress(input, input_size, compressed, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        return result;
    }
    
    uint8_t decompressed[MAX_OUTPUT_SIZE];
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, MAX_OUTPUT_SIZE);
    
    if (decompressed_size != input_size) {
        return result;
    }
    
    if (memcmp(input, decompressed, input_size) != 0) {
        return result;
    }
    
    result.data = malloc(compressed_size);
    if (result.data == NULL) {
        return result;
    }
    
    memcpy(result.data, compressed, compressed_size);
    result.size = compressed_size;
    return result;
}

int main() {
    CompressFunc compress_funcs[] = {null_compress, run_length_encode};
    DecompressFunc decompress_funcs[] = {null_decompress, run_length_decode};
    const char *method_names[] = {"Null", "Run-length"};
    size_t num_methods = sizeof(compress_funcs) / sizeof(compress_funcs[0]);
    
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    size_t test_size = sizeof(test_data);
    
    printf("Original data: ");
    for (size_t i = 0; i < test_size; i++) {
        printf("%d ", test_data[i]);
    }
    printf("\nSize: %zu bytes\n\n", test_size);
    
    for (size_t i = 0; i < num_methods; i++) {
        printf("Testing %s compression:\n", method_names[i]);
        
        Buffer result = process_data(test_data, test_size, compress_funcs[i], decompress_funcs[i]);
        
        if (result.data == NULL) {
            printf("  Compression failed\n");
            continue;
        }
        
        printf("  Compressed size: %zu bytes\n", result.size);
        printf("  Compression ratio: %.2f%%\n", (1.0 - (double)result.size / test_size) * 100);