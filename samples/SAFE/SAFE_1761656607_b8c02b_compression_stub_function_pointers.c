//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size);

size_t run_length_encode(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer compress_data(const uint8_t* data, size_t size, CompressFunc compress) {
    Buffer result = {NULL, 0};
    if (data == NULL || size == 0 || compress == NULL) return result;
    
    size_t max_output_size = size * 2 + 1;
    uint8_t* compressed = malloc(max_output_size);
    if (compressed == NULL) return result;
    
    size_t compressed_size = compress(data, size, compressed, max_output_size);
    if (compressed_size == 0 || compressed_size > max_output_size) {
        free(compressed);
        return result;
    }
    
    result.data = compressed;
    result.size = compressed_size;
    return result;
}

Buffer decompress_data(const uint8_t* data, size_t size, DecompressFunc decompress, size_t expected_size) {
    Buffer result = {NULL, 0};
    if (data == NULL || size == 0 || decompress == NULL || expected_size == 0) return result;
    
    uint8_t* decompressed = malloc(expected_size);
    if (decompressed == NULL) return result;
    
    size_t decompressed_size = decompress(data, size, decompressed, expected_size);
    if (decompressed_size != expected_size) {
        free(decompressed);
        return result;
    }
    
    result.data = decompressed;
    result.size = decompressed_size;
    return result;
}

int main() {
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 5, 5};
    size_t test_size = sizeof(test_data);
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Original data: ");
    for (size_t i = 0; i < test_size; i++) {
        printf("%d ", test_data[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_data(test_data, test_size, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%d ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress_func, test_size);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed data: ");
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%d ", decompressed.data[i]);
    }
    printf("\n");
    
    int match = memcmp(test_data, decompressed.data, test_size) == 0;
    printf("Data matches original: %s\n", match ? "yes" : "no");
    
    free(compressed.data);
    free(decompressed.data);
    
    return match ? 0 : 1;
}