//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
        
        output[out_idx++] = (uint8_t)count;
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
        
        if (out_idx + count > output_size) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

Buffer compress_data(const uint8_t *input, size_t input_size, CompressFunc compress) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || compress == NULL) {
        return result;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t compressed_size = compress(input, input_size, output, MAX_OUTPUT_SIZE);
    
    if (compressed_size > 0 && compressed_size <= MAX_OUTPUT_SIZE) {
        result.data = malloc(compressed_size);
        if (result.data != NULL) {
            memcpy(result.data, output, compressed_size);
            result.size = compressed_size;
        }
    }
    
    free(output);
    return result;
}

Buffer decompress_data(const uint8_t *input, size_t input_size, DecompressFunc decompress) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE || decompress == NULL) {
        return result;
    }
    
    uint8_t *output = malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return result;
    }
    
    size_t decompressed_size = decompress(input, input_size, output, MAX_OUTPUT_SIZE);
    
    if (decompressed_size > 0 && decompressed_size <= MAX_OUTPUT_SIZE) {
        result.data = malloc(decompressed_size);
        if (result.data != NULL) {
            memcpy(result.data, output, decompressed_size);
            result.size = decompressed_size;
        }
    }
    
    free(output);
    return result;
}

void process_compression_demo(void) {
    uint8_t test_data[] = "AAAABBBCCDAA";
    size_t test_size = sizeof(test_data) - 1;
    
    CompressFunc compressors[] = {run_length_encode};
    DecompressFunc decompressors[] = {run_length_decode};
    const char *names[] = {"Run-length encoding"};
    
    for (size_t i = 0; i < 1; i++) {
        printf("Testing %s:\n", names[i]);
        printf("Original: %s\n", test_data);
        printf("Original size: %zu\n", test_size);
        
        Buffer compressed = compress_data(test_data, test_size, compressors[i]);
        if (compressed.data != NULL && compressed.size > 0) {
            printf("Compressed size: %zu\n", compressed.size);
            
            Buffer decompressed = decompress_data(compressed.data, compressed.size, decompressors[i]);
            if (decompressed.data != NULL && decompressed.size > 0) {
                printf("Decompressed: ");
                for (size_t j = 0; j < decompressed.size; j++) {
                    printf("%c", decompressed.data[j]);
                }
                printf("\n");
                printf("Decompressed size: %zu\n", decompressed.size);
                
                if (decompressed.size == test_size && memcmp(test_data, decompressed.data, test_size) == 0) {
                    printf("Success: Data matches original!\n");
                } else {
                    printf("Error: Data doesn't match original!\n");
                }
                
                free(decompressed.data);
            } else {
                printf("Error: Decompression failed!\n");
            }
            free