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

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
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

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || compress == NULL) return result;
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) return result;
    
    size_t compressed_size = compress(input, input_size, output, MAX_OUTPUT_SIZE);
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.data = output;
        result.size = compressed_size;
    } else {
        free(output);
    }
    
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress) {
    Buffer result = {NULL, 0};
    if (input == NULL || input_size == 0 || decompress == NULL) return result;
    
    uint8_t *output = malloc(MAX_INPUT_SIZE);
    if (output == NULL) return result;
    
    size_t decompressed_size = decompress(input, input_size, output, MAX_INPUT_SIZE);
    if (decompressed_size > 0 && decompressed_size <= MAX_INPUT_SIZE) {
        result.data = output;
        result.size = decompressed_size;
    } else {
        free(output);
    }
    
    return result;
}

int main() {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while (input_size < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input_data[input_size++] = (uint8_t)c;
    }
    
    if (input_size == 0) {
        printf("No input data provided.\n");
        return 1;
    }
    
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    Buffer compressed = compress_data(input_data, input_size, compress_func);
    if (compressed.data == NULL) {
        printf("Compression failed.\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100.0f);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size, decompress_func);
    if (decompressed.data == NULL) {
        printf("Decompression failed.\n");
        free(compressed.data);
        return 1;
    }
    
    if (decompressed.size == input_size && memcmp(input_data, decompressed.data, input_size) == 0) {
        printf("Decompression successful: data matches original.\n");
    } else {
        printf("Decompression failed: data does not match original.\n");
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < input_size && i < 50; i++) {
        printf("%c", input_data[i]);
    }
    if (input_size > 50) printf("...");
    printf("\n");
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}